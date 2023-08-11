#pragma once
#include <wdm.h>
#include <wdf.h>
#include <hidport.h>

#include <initguid.h>
#include <hidport.h>

NTSTATUS QueueInitialize(_In_ WDFDEVICE Device);
VOID EvtWdfIoQueueIoInternalDeviceControl(_In_ WDFQUEUE, _In_ WDFREQUEST, _In_ size_t, _In_ size_t, _In_ ULONG);
