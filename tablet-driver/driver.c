#include "Driver.h"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	NTSTATUS status = STATUS_SUCCESS;

	WDF_DRIVER_CONFIG config;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);

	status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
	if (!NT_SUCCESS(status))
		return status;
	return status;
}

NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit) {
	UNREFERENCED_PARAMETER(Driver);
	NTSTATUS status = STATUS_SUCCESS;

	WdfFdoInitSetFilter(DeviceInit);

	////////////////////////////////////////DEVICE
	WDF_OBJECT_ATTRIBUTES attribute;
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, CONTEXT);

	WDFDEVICE device = NULL;
	status = WdfDeviceCreate(&DeviceInit, &attribute, &device);
	if (!NT_SUCCESS(status))
		return status;

	////////////////////////////////////////QUEUE
	WDF_IO_QUEUE_CONFIG config;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&config, WdfIoQueueDispatchParallel);
	config.EvtIoInternalDeviceControl = EvtIoInternalDeviceControl;

	WDFQUEUE queue;
	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (!NT_SUCCESS(status))
		return status;

	////////////////////////////////////////QUEUE2
	WDF_IO_QUEUE_CONFIG_INIT(&config, WdfIoQueueDispatchManual);
	config.PowerManaged = WdfFalse; //i dont know

	PCONTEXT context = DeviceGetContext(device);
	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &context->_queue);
	if (!NT_SUCCESS(status))
		return status;

	return status;
}

VOID EvtIoInternalDeviceControl(_In_ WDFQUEUE queue, _In_ WDFREQUEST request,
	_In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);
	NTSTATUS status = STATUS_NOT_SUPPORTED;
	BOOLEAN complete = TRUE;

	switch (IoControlCode) {
	case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
		status = RequestRetrieveOutputBuffer(request);
		break;
	case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
		status = MemoryCopyFromBuffer(request, &g_descriptor, g_descriptor.bLength);
		break;
	case IOCTL_HID_GET_REPORT_DESCRIPTOR:
		status = MemoryCopyFromBuffer(request, &g_reportDescriptor, g_descriptor.DescriptorList[0].wReportLength);
		break;
	case IOCTL_HID_READ_REPORT:
	case IOCTL_HID_GET_INPUT_REPORT:
		status = RequestForwardToIoQueue(queue, request, &complete);
		break;
	case IOCTL_HID_WRITE_REPORT:
	case IOCTL_HID_SET_OUTPUT_REPORT:
		status = RequestGetIrp(queue, request);
		break;
	}

	if (complete)
		WdfRequestComplete(request, status);
}

NTSTATUS RequestRetrieveOutputBuffer(_In_ WDFREQUEST request) {
	NTSTATUS status = STATUS_SUCCESS;
	PHID_DEVICE_ATTRIBUTES attribute = NULL;

	status = WdfRequestRetrieveOutputBuffer(request, sizeof(HID_DEVICE_ATTRIBUTES), &attribute, NULL);
	if (!NT_SUCCESS(status))
		return status;

	attribute->Size = sizeof(HID_DEVICE_ATTRIBUTES);
	attribute->VendorID = 0x00EE;
	attribute->ProductID = 0xBACC;
	attribute->VersionNumber = 0x0001;

	WdfRequestSetInformation(request, sizeof(HID_DEVICE_ATTRIBUTES));
	return status;
}

NTSTATUS MemoryCopyFromBuffer(_In_ WDFREQUEST request, _In_ PVOID buffer, _In_ size_t length) {
	//if (length <= 0)
	//	return STATUS_INVALID_BUFFER_SIZE;
	NTSTATUS status = STATUS_SUCCESS;

	WDFMEMORY memory = NULL;
	status = WdfRequestRetrieveOutputMemory(request, &memory);
	if (!NT_SUCCESS(status))
		return status;

	//size_t size = 0;
	//WdfMemoryGetBuffer(memory, &size);
	//if (size < length)
	//	return STATUS_INVALID_BUFFER_SIZE;

	status = WdfMemoryCopyFromBuffer(memory, 0, buffer, length);
	if (!NT_SUCCESS(status))
		return status;

	WdfRequestSetInformation(request, length);
	return status;
}

NTSTATUS RequestForwardToIoQueue(_In_ WDFQUEUE queue, _In_ WDFREQUEST request, OUT BOOLEAN* complete) {
	NTSTATUS status = STATUS_SUCCESS;

	WDFDEVICE device = WdfIoQueueGetDevice(queue);
	PCONTEXT context = DeviceGetContext(device);

	status = WdfRequestForwardToIoQueue(request, context->_queue);
	if (NT_SUCCESS(status))
		*complete = FALSE;

	return status;
}

NTSTATUS RequestGetIrp(_In_ WDFQUEUE queue, _In_ WDFREQUEST request) {
	NTSTATUS status = STATUS_SUCCESS;

	WDF_REQUEST_PARAMETERS param;
	WDF_REQUEST_PARAMETERS_INIT(&param);
	WdfRequestGetParameters(request, &param);

	if (param.Parameters.DeviceIoControl.InputBufferLength < sizeof(HID_XFER_PACKET))
		return STATUS_BUFFER_TOO_SMALL;

	PHID_XFER_PACKET packet;
	packet = (PHID_XFER_PACKET)WdfRequestWdmGetIrp(request)->UserBuffer;

	switch (packet->reportId) {
	case REPORT_ID_MOUSE_OUTPUT:
		packet->reportBuffer[0] = REPORT_ID_MOUSE_INPUT;
		break;
	default:
		return STATUS_INVALID_PARAMETER;
	}

	/////////////////////////////////////////////////////////////////

	WDFDEVICE device = WdfIoQueueGetDevice(queue);
	PCONTEXT context = DeviceGetContext(device);
	WDFREQUEST _request = NULL;
	status = WdfIoQueueRetrieveNextRequest(context->_queue, &_request);
	if (!NT_SUCCESS(status))
		return status;

	//1
	status = MemoryCopyFromBuffer(_request, packet->reportBuffer, packet->reportBufferLen);
	WdfRequestComplete(_request, status);

	//2
	//PVOID buffer = NULL;
	//status = WdfRequestRetrieveOutputBuffer(_request, packet->reportBufferLen, &buffer, NULL);
	//if (!NT_SUCCESS(status))
	//	return status;
	//RtlCopyMemory(buffer, packet->reportBuffer, packet->reportBufferLen);
	//WdfRequestCompleteWithInformation(_request, status, packet->reportBufferLen);

	WdfRequestSetInformation(request, packet->reportBufferLen);
	return status;
}