#include "queue_manual.h"
#include "device.h"

NTSTATUS QueueInitialize2(_In_ WDFDEVICE device) {
	NTSTATUS status = STATUS_SUCCESS;

	WDF_IO_QUEUE_CONFIG config;
	WDF_IO_QUEUE_CONFIG_INIT(&config, WdfIoQueueDispatchManual);
	config.PowerManaged = WdfFalse;

	PDEVICE_CONTEXT context = DeviceGetContext(device);
	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &context->_queue);
	if (!NT_SUCCESS(status))
		return status;

	return status;
}