#pragma once
#include "main.h"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	NTSTATUS status = STATUS_SUCCESS;
	WDF_OBJECT_ATTRIBUTES attribute;
	WDF_DRIVER_CONFIG config;

	WDF_OBJECT_ATTRIBUTES_INIT(&attribute);
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);

	status = WdfDriverCreate(DriverObject, RegistryPath, &attribute, &config, WDF_NO_HANDLE);
	if (!NT_SUCCESS(status))
		return status;
	return status;
}

NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit) {
	UNREFERENCED_PARAMETER(Driver);
	NTSTATUS status = STATUS_SUCCESS;

	////////////////////////////////////////////////DEVICE
	//WDF_OBJECT_ATTRIBUTES attribute;
	WDFDEVICE device;

	//WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);

	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (!NT_SUCCESS(status))
		return status;

	////////////////////////////////////////////////SYMBOLIC
	DECLARE_CONST_UNICODE_STRING(symName, L"\\DosDevices\\tablet-symbolic");
	WdfDeviceCreateSymbolicLink(device, &symName);

	////////////////////////////////////////////////INTERFACE
	status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_tabletdriver, NULL);
	if (!NT_SUCCESS(status))
		return status;

	////////////////////////////////////////////////QUEUE
	WDF_IO_QUEUE_CONFIG config;
	WDFQUEUE queue;

	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&config, WdfIoQueueDispatchParallel);
	//config.EvtIoInternalDeviceControl = EvtWdfIoQueueIoInternalDeviceControl;
	config.EvtIoWrite = EvtIoWrite;

	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (!NT_SUCCESS(status))
		return status;

	return status;
}

//VOID EvtWdfIoQueueIoInternalDeviceControl(_In_ WDFQUEUE queue, _In_ WDFREQUEST request,
//	_In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
//	UNREFERENCED_PARAMETER(queue);
//	UNREFERENCED_PARAMETER(OutputBufferLength);
//	UNREFERENCED_PARAMETER(InputBufferLength);
//
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "EvtWdfIoQueueIoInternalDeviceControl\n");
//
//	NTSTATUS status = STATUS_SUCCESS;
//	//WDFDEVICE device = WdfIoQueueGetDevice(queue);
//	//PDEVICE_CONTEXT context = DeviceGetContext(device);
//
//	WDF_REQUEST_PARAMETERS params;
//	switch (IoControlCode) {
//	case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
//	case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
//	case IOCTL_HID_GET_REPORT_DESCRIPTOR:
//	case IOCTL_HID_GET_STRING:
//		break;
//	case IOCTL_HID_WRITE_REPORT:
//	case IOCTL_HID_SET_OUTPUT_REPORT:
//		WDF_REQUEST_PARAMETERS_INIT(&params);
//		WdfRequestGetParameters(request, &params);
//		params.Parameters.DeviceIoControl.InputBufferLength;
//		DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "IOCTL_HID_WRITE_REPORT++\n");
//		break;
//	case IOCTL_HID_READ_REPORT:
//	case IOCTL_HID_GET_INPUT_REPORT:
//		break;
//	case IOCTL_HID_SET_FEATURE:
//		break;
//	case IOCTL_HID_GET_FEATURE:
//		break;
//	case IOCTL_HID_ACTIVATE_DEVICE:
//	case IOCTL_HID_DEACTIVATE_DEVICE:
//	default:
//		status = STATUS_NOT_SUPPORTED;
//		break;
//	}
//
//	WdfRequestComplete(request, status);
//}


VOID EvtIoWrite(_In_ WDFQUEUE queue, _In_ WDFREQUEST request, _In_ size_t length) {
	UNREFERENCED_PARAMETER(queue);
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "EvtIoWrite\n");

	//1
	//WDFMEMORY memory;
	//status = WdfRequestRetrieveInputMemory(request, &memory);
	//WdfMemoryCopyToBuffer(memory, 0, length, dwReturnSize);

	//2
	WDF_REQUEST_PARAMETERS params;
	WDF_REQUEST_PARAMETERS_INIT(&params);
	WdfRequestGetParameters(request, &params);


	PREPROT report = (PREPROT)WdfRequestWdmGetIrp(request)->UserBuffer;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "VmultiID: %d\n", report->vmultiId);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "ReportLen: %d\n", report->reportLen);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "ReportID: %d\n", report->reportId);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Button: %d\n", report->button);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "X: %hd\n", report->x);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Y: %hd\n", report->y);

	//WDF_REQUEST_PARAMETERS_INIT(&params);
	//WdfRequestGetParameters(request, &params);
	//params.Parameters.DeviceIoControl.InputBufferLength;
	//DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "IOCTL_HID_WRITE_REPORT++\n");

	//WDFMEMORY memory;
	//status = WdfRequestRetrieveInputMemory(request, &memory);
	//if (NT_SUCCESS(status)) {
	//}

	WdfRequestCompleteWithInformation(request, status, length);
}