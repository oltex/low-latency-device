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
	NTSTATUS status = STATUS_SUCCESS;
	////////////////////////////////////////////////DEVICE
	WDF_OBJECT_ATTRIBUTES attribute;
	WDFDEVICE device;

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);

	status = WdfDeviceCreate(&DeviceInit, &attribute, &device);
	if (!NT_SUCCESS(status))
		return status;

	////////////////////////////////////////////////QUEUE
	WDF_IO_QUEUE_CONFIG config;
	WDFQUEUE queue;

	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&config, WdfIoQueueDispatchParallel);
	config.EvtIoInternalDeviceControl = EvtWdfIoQueueIoInternalDeviceControl;

	status = WdfIoQueueCreate(device, &config, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (!NT_SUCCESS(status))
		return status;

	return status;
}

VOID EvtWdfIoQueueIoInternalDeviceControl(_In_ WDFQUEUE queue, _In_ WDFREQUEST request,
	_In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(InputBufferLength);

	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device = WdfIoQueueGetDevice(queue);
	PDEVICE_CONTEXT context = VMultiGetDeviceContext(device);

	switch (IoControlCode) {

	}
}

/////////////////////////////////////////////////////////////

NTSTATUS FIRSTKMDFCreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit) {
	WDF_OBJECT_ATTRIBUTES   deviceAttributes;
	PDEVICE_CONTEXT deviceContext;
	WDFDEVICE device;
	NTSTATUS status;

	PAGED_CODE();

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

	status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

	if (NT_SUCCESS(status)) {
		deviceContext = DeviceGetContext(device);

		deviceContext->dwStringLength = 0;
		DECLARE_CONST_UNICODE_STRING(symName, SYMBOLIC_NAME);
		WdfDeviceCreateSymbolicLink(device, &symName);

		status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_FIRSTKMDF, NULL);

		if (NT_SUCCESS(status)) {
			status = FIRSTKMDFQueueInitialize(device);
		}
	}

	return status;
}

NTSTATUS FIRSTKMDFQueueInitialize(_In_ WDFDEVICE Device) {
	WDFQUEUE queue;
	NTSTATUS status;
	WDF_IO_QUEUE_CONFIG    queueConfig;

	PAGED_CODE();

	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);

	queueConfig.EvtIoRead = FIRSTKMDFEvtRead;
	queueConfig.EvtIoWrite = FIRSTKMDFEvtWrite;

	queueConfig.EvtIoDeviceControl = FIRSTKMDFEvtIoDeviceControl;
	queueConfig.EvtIoStop = FIRSTKMDFEvtIoStop;

	status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);

	if (!NT_SUCCESS(status))
		return status;

	return status;
}

VOID
FIRSTKMDFEvtRead(
	_In_ WDFQUEUE Queue,
	_In_ WDFREQUEST Request,
	_In_ size_t Length
)
{
	NTSTATUS    status = STATUS_SUCCESS;
	WDFMEMORY memory;
	PDEVICE_CONTEXT deviceContext;
	SIZE_T dwReturnSize = 0;

	deviceContext = DeviceGetContext(WdfIoQueueGetDevice(Queue));

	status = WdfRequestRetrieveOutputMemory(Request, &memory);
	if (NT_SUCCESS(status)) {
		// 적합한 읽기를 위한 문자열크기를 계산합니다
		dwReturnSize = (deviceContext->dwStringLength > Length) ?
			Length : deviceContext->dwStringLength;

		if (dwReturnSize)
		{
			WdfMemoryCopyFromBuffer(memory, 0, deviceContext->pInternalStringBuffer, dwReturnSize);
		}
	}
	WdfRequestCompleteWithInformation(Request, status, dwReturnSize);
}

VOID
FIRSTKMDFEvtWrite(
	_In_ WDFQUEUE Queue,
	_In_ WDFREQUEST Request,
	_In_ size_t Length
)
{
	NTSTATUS    status = STATUS_SUCCESS;
	WDFMEMORY memory;
	PDEVICE_CONTEXT deviceContext;
	SIZE_T dwReturnSize = 0;

	deviceContext = DeviceGetContext(WdfIoQueueGetDevice(Queue));

	status = WdfRequestRetrieveInputMemory(Request, &memory);
	if (NT_SUCCESS(status)) {
		if (deviceContext->dwStringLength) // 이전 문자열이 보관되어 있다면,
		{
			ExFreePool(deviceContext->pInternalStringBuffer); // 우선 문자열을 보관하는 용도로 사용되던 버퍼를 해제합니다
			deviceContext->pInternalStringBuffer = NULL;
			deviceContext->dwStringLength = 0;
		}

		deviceContext->dwStringLength = Length;

		if (deviceContext->dwStringLength)
		{
			// 다시 새로운 버퍼를 할당합니다
			deviceContext->pInternalStringBuffer = ExAllocatePool(
				NonPagedPool,
				deviceContext->dwStringLength);

			// 사용자가 전달한 버퍼의 내용을 복사합니다
			dwReturnSize = deviceContext->dwStringLength;
			WdfMemoryCopyToBuffer(memory, 0, deviceContext->pInternalStringBuffer, dwReturnSize);
		}
	}
	WdfRequestCompleteWithInformation(Request, status, dwReturnSize);
}