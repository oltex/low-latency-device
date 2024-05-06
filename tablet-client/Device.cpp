#include "Device.h"

#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>

Device::Device(Config const& config) noexcept {
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);

	HDEVINFO devInfo = SetupDiGetClassDevs(&hidGuid, NULL, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	SP_DEVICE_INTERFACE_DATA devItfData;
	devItfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	DWORD idx = 0;
	while (INVALID_HANDLE_VALUE == _handle) {
		SetupDiEnumInterfaceDevice(devInfo, NULL, &hidGuid, idx++, &devItfData);
		if (ERROR_NO_MORE_ITEMS == GetLastError())
			break;

		DWORD size;
		SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, NULL, 0, &size, NULL);
		PSP_DEVICE_INTERFACE_DETAIL_DATA devItfDetailData;
		devItfDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);
		devItfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, devItfDetailData, size, &size, NULL);

		_handle = CreateFile(devItfDetailData->DevicePath, config.desiredAccess, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (INVALID_HANDLE_VALUE != _handle) {
			HIDD_ATTRIBUTES attributes;
			PHIDP_PREPARSED_DATA preparsedData;
			HIDP_CAPS capabilities;
			HidD_GetAttributes(_handle, &attributes);
			HidD_GetPreparsedData(_handle, &preparsedData);
			HidP_GetCaps(preparsedData, &capabilities);

			if (attributes.VendorID == config.vendorId &&
				attributes.ProductID == config.productId &&
				capabilities.UsagePage == config.usagePage &&
				capabilities.Usage == config.usage) {

				COMMTIMEOUTS commTimeOuts;
				GetCommTimeouts(_handle, &commTimeOuts);
				commTimeOuts.ReadIntervalTimeout = 1;
				commTimeOuts.ReadTotalTimeoutConstant = 1;
				commTimeOuts.ReadTotalTimeoutMultiplier = 1;
				SetCommTimeouts(_handle, &commTimeOuts);
			}
			else {
				CloseHandle(_handle);
				_handle = INVALID_HANDLE_VALUE;
			}
			HidD_FreePreparsedData(preparsedData);
		}
		free(devItfDetailData);
	}
	SetupDiDestroyDeviceInfoList(devInfo);
}

Device::~Device(void) {
	CloseHandle(_handle);
}

void Device::Read(void* const /*__restrict*/ buf, unsigned char const len) const noexcept {
	ReadFile(_handle, buf, len, NULL, NULL);
}

void Device::Write(void const* const /*__restrict*/ buf, unsigned char const len) const noexcept {
	WriteFile(_handle, buf, len, NULL, NULL);
}