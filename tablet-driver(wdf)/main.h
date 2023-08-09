#pragma once
#include <wdm.h>
#include <wdf.h>

typedef struct _DEVICE_CONTEXT {
    WDFQUEUE ReportQueue;
    BYTE DeviceMode;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, VMultiGetDeviceContext)