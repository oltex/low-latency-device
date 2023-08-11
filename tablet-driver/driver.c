#include "driver.h"
#include "device.h"

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

	//UCHAR minorFunction = IRP_MN_QUERY_ID;
	//status = WdfDeviceInitAssignWdmIrpPreprocessCallback(DeviceInit, VMultiEvtWdmPreprocessMnQueryId, IRP_MJ_PNP, &minorFunction, 1);

	status = CreateDevice(DeviceInit);
	////////////////////////////////////////////////INTERFACE
	//status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_tabletdriver, NULL);
	//if (!NT_SUCCESS(status))
	//	return status;

	return status;
}



//
//VOID EvtIoWrite(_In_ WDFQUEUE queue, _In_ WDFREQUEST request, _In_ size_t length) {
//	//UNREFERENCED_PARAMETER(queue);
//	NTSTATUS status = STATUS_SUCCESS;
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "EvtIoWrite\n");
//
//	//1
//	REPORT report = { 0 };
//	WDFMEMORY memory;
//	status = WdfRequestRetrieveInputMemory(request, &memory);
//	WdfMemoryCopyToBuffer(memory, 0, (PVOID)&report, sizeof(REPORT));
//
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "VmultiID: %d\n", report.vmultiId);
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "ReportLen: %d\n", report.reportLen);
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "ReportID: %d\n", report.reportId);
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Button: %d\n", report.button);
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "X: %hd\n", report.x);
//	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Y: %hd\n", report.y);
//
//	//2
//	WDFDEVICE device = WdfIoQueueGetDevice(queue);
//	PDEVICE_CONTEXT context = DeviceGetContext(device);
//	WDFREQUEST request2;
//	NTSTATUS status2 = STATUS_SUCCESS;
//
//	status2 = WdfIoQueueRetrieveNextRequest(context->queue, &request2);
//	if (NT_SUCCESS(status2)) {
//		DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "WdfIoQueueRetrieveNextRequest\n");
//		PVOID pReadReport = NULL;
//		size_t bytesReturned = 0;
//
//		status2 = WdfRequestRetrieveOutputBuffer(request2, length, &pReadReport, &bytesReturned);
//		if (NT_SUCCESS(status2)) {
//			DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "WdfRequestRetrieveOutputBuffer\n");
//
//			if (bytesReturned > sizeof(REPORT))
//				bytesReturned = sizeof(REPORT);
//
//			RtlCopyMemory(pReadReport, (PVOID)&report, bytesReturned);
//
//			WdfRequestCompleteWithInformation(request2, status2, bytesReturned);
//		}
//	}
//
//	WdfRequestCompleteWithInformation(request, status, length);
//}