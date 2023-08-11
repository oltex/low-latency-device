#include "device.h"
#include "queue.h"
#include "queue_manual.h"

NTSTATUS CreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit) {
	NTSTATUS status = STATUS_SUCCESS;

	WDF_OBJECT_ATTRIBUTES attribute;
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);

	WDFDEVICE device = NULL;
	status = WdfDeviceCreate(&DeviceInit, &attribute, &device);
	if (!NT_SUCCESS(status))
		return status;

	DECLARE_CONST_UNICODE_STRING(symName, L"\\DosDevices\\tablet-symbolic");
	WdfDeviceCreateSymbolicLink(device, &symName);

	status = QueueInitialize(device);
	if (!NT_SUCCESS(status))
		return status;

	status = QueueInitialize2(device);
	if (!NT_SUCCESS(status))
		return status;

	return status;
}