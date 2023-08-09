#include "Device.h"

Device::Device(const Config& config) {
	GUID hidGuid;
	HDEVINFO devInfo;

	SP_DEVICE_INTERFACE_DATA devItfData;
	devItfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	PSP_DEVICE_INTERFACE_DETAIL_DATA devItfDetailData = nullptr;

	HIDD_ATTRIBUTES attributes;
	PHIDP_PREPARSED_DATA preparsedData = nullptr;
	HIDP_CAPS hidCapabilities;

	DWORD memberIdx = 0, size;

	HidD_GetHidGuid(&hidGuid);
	devInfo = SetupDiGetClassDevs(&hidGuid, NULL, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	while (_handle == nullptr || _handle == INVALID_HANDLE_VALUE) {
		SetupDiEnumDeviceInterfaces(devInfo, NULL, &hidGuid, memberIdx++, &devItfData);
		if (ERROR_NO_MORE_ITEMS == GetLastError())
			break;

		SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, NULL, 0, &size, NULL);
		devItfDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);
		devItfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, devItfDetailData, size, &size, NULL);

		_handle = CreateFile(devItfDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE != _handle) {
			HidD_GetAttributes(_handle, &attributes);
			HidD_GetPreparsedData(_handle, &preparsedData);
			HidP_GetCaps(preparsedData, &hidCapabilities);

			if (attributes.VendorID == config.vendorId &&
				attributes.ProductID == config.productId &&
				hidCapabilities.UsagePage == config.usagePage &&
				hidCapabilities.Usage == config.usage) {

				COMMTIMEOUTS commTimeOuts;
				GetCommTimeouts(_handle, &commTimeOuts);
				commTimeOuts.ReadIntervalTimeout = 1;
				commTimeOuts.ReadTotalTimeoutConstant = 1;
				commTimeOuts.ReadTotalTimeoutMultiplier = 1;
				SetCommTimeouts(_handle, &commTimeOuts);
			}
			else {
				CloseHandle(_handle);
				_handle = nullptr;
			}
			HidD_FreePreparsedData(preparsedData);
		}
		free(devItfDetailData);
	}
	SetupDiDestroyDeviceInfoList(devInfo);
}

Device::~Device(void) {
	CloseHandle(_handle);
	_handle = nullptr;
}

int Device::Read(void* const buf, const BYTE& len) noexcept {
	register DWORD byte;
	if (ReadFile(_handle, buf, len, &byte, NULL))
		return byte;
	return 0;
}

int Device::Write(const void* const buf, const BYTE& len) noexcept {
	register DWORD byte;
	if (WriteFile(_handle, buf, len, &byte, NULL))
		return byte;
	return 0;
}
