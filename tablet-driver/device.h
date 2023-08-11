#pragma once
#include <wdm.h>
#include <wdf.h>
#include <hidport.h>

#include <initguid.h>
#include <hidport.h>

NTSTATUS CreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit);

typedef struct _DEVICE_CONTEXT {
    WDFQUEUE _queue;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

