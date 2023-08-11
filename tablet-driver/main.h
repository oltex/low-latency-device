#pragma once
#include <wdm.h>
#include <wdf.h>

#include <initguid.h>
#include <hidport.h>

typedef struct _DEVICE_CONTEXT {
	BYTE DeviceMode;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

DEFINE_GUID(GUID_DEVINTERFACE_tabletdriver, 0x745a17a0, 0x74d3, 0x11d0, 0xb6, 0xfe, 0x00, 0xa0, 0xc9, 0x0f, 0x57, 0xda);

typedef struct Report {
	BYTE vmultiId, reportLen, reportId;
	UCHAR button;
	USHORT x, y;
}REPORT, *PREPROT;

NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER, _Inout_ PWDFDEVICE_INIT);
VOID EvtWdfIoQueueIoInternalDeviceControl(_In_ WDFQUEUE, _In_ WDFREQUEST, _In_ size_t, _In_ size_t, _In_ ULONG);
VOID EvtIoWrite(_In_ WDFQUEUE, _In_ WDFREQUEST, _In_ size_t);