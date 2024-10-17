#pragma once
#include <Windows.h>

#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>

class device final {
public:
	struct config final {
		unsigned long const desired_access;
		unsigned short const vendor_id, product_id;
		unsigned short const usage_page, usage;
	};
public:
	inline explicit device(config const& config) noexcept {
		GUID hidGuid;
		HidD_GetHidGuid(&hidGuid);

		HDEVINFO devInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

		DWORD idx = 0;
		while (INVALID_HANDLE_VALUE == _handle) {
			SP_DEVICE_INTERFACE_DATA devItfData;
			devItfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			SetupDiEnumInterfaceDevice(devInfo, NULL, &hidGuid, idx++, &devItfData);
			if (ERROR_NO_MORE_ITEMS == GetLastError())
				break;

			DWORD size;
			SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, NULL, 0, &size, NULL);
			PSP_DEVICE_INTERFACE_DETAIL_DATA devItfDetailData;
			devItfDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);
			devItfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetail(devInfo, &devItfData, devItfDetailData, size, &size, NULL);

			_handle = CreateFile(devItfDetailData->DevicePath, config.desired_access, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL);

			if (INVALID_HANDLE_VALUE != _handle) {
				HIDD_ATTRIBUTES attributes;
				PHIDP_PREPARSED_DATA preparsedData;
				HIDP_CAPS capabilities;
				HidD_GetAttributes(_handle, &attributes);
				HidD_GetPreparsedData(_handle, &preparsedData);
				HidP_GetCaps(preparsedData, &capabilities);

				if (attributes.VendorID == config.vendor_id &&
					attributes.ProductID == config.product_id &&
					capabilities.UsagePage == config.usage_page &&
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
	};
	inline ~device(void) noexcept {
		CloseHandle(_handle);
	};
public:
	inline void read(void* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		ReadFile(_handle, buffer, length, nullptr, nullptr);
	}
	inline void write(void const* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		WriteFile(_handle, buffer, length, nullptr, nullptr);
	}
private:
	HANDLE /*__restrict*/ _handle = INVALID_HANDLE_VALUE;
};