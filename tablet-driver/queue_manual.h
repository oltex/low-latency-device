#pragma once
#include <wdm.h>
#include <wdf.h>
#include <hidport.h>

#include <initguid.h>
#include <hidport.h>

NTSTATUS QueueInitialize2(_In_ WDFDEVICE Device);