#include "queue.h"
#include "device.h"

NTSTATUS QueueInitialize(_In_ WDFDEVICE device) {
	NTSTATUS status = STATUS_SUCCESS;

	WDF_IO_QUEUE_CONFIG config;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&config, WdfIoQueueDispatchParallel);
	config.EvtIoInternalDeviceControl = EvtWdfIoQueueIoInternalDeviceControl;

	WDFQUEUE queue;
	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (!NT_SUCCESS(status))
		return status;
	return status;
}

VOID EvtWdfIoQueueIoInternalDeviceControl(_In_ WDFQUEUE queue, _In_ WDFREQUEST request,
	_In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
	UNREFERENCED_PARAMETER(queue);
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "EvtWdfIoQueueIoInternalDeviceControl\n");

	NTSTATUS status = STATUS_SUCCESS;
	//WDFDEVICE device = WdfIoQueueGetDevice(queue);
	//PDEVICE_CONTEXT context = DeviceGetContext(device);

	//WDF_REQUEST_PARAMETERS params;
	//WDF_REQUEST_PARAMETERS_INIT(&params);
	//WdfRequestGetParameters(request, &params);
	//params.Parameters.DeviceIoControl.InputBufferLength;

	switch (IoControlCode) {
	case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
	case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
	case IOCTL_HID_GET_REPORT_DESCRIPTOR:
	case IOCTL_HID_GET_STRING:
		break;
	case IOCTL_HID_WRITE_REPORT:
	case IOCTL_HID_SET_OUTPUT_REPORT:
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "IOCTL_HID_WRITE_REPORT\n");

		break;
	case IOCTL_HID_READ_REPORT:
	case IOCTL_HID_GET_INPUT_REPORT:
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "IOCTL_HID_READ_REPORT\n");

		//WDF_REQUEST_PARAMETERS_INIT(&params);
		//WdfRequestGetParameters(Request, &params);
		break;
	case IOCTL_HID_SET_FEATURE:
		break;
	case IOCTL_HID_GET_FEATURE:
		break;
	case IOCTL_HID_ACTIVATE_DEVICE:
	case IOCTL_HID_DEACTIVATE_DEVICE:
	default:
		status = STATUS_NOT_SUPPORTED;
		break;
	}

	WdfRequestComplete(request, status);
}