#pragma once
#include <Windows.h>

#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>

#include <iostream>
class device final {
public:
	struct config final {
		unsigned long const desired_access;
		unsigned short const vendor_id, product_id;
		unsigned short const usage_page, usage;
	};
public:
	inline explicit device(config const& config) noexcept {
		GUID guid;
		HidD_GetHidGuid(&guid);

		HDEVINFO dev_info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

		DWORD index = 0;
		while (INVALID_HANDLE_VALUE == _handle) {
			SP_DEVICE_INTERFACE_DATA devItf_data;
			devItf_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			SetupDiEnumInterfaceDevice(dev_info, NULL, &guid, index++, &devItf_data);
			if (ERROR_NO_MORE_ITEMS == GetLastError())
				break;

			DWORD size;
			SetupDiGetDeviceInterfaceDetail(dev_info, &devItf_data, NULL, 0, &size, NULL);
			PSP_DEVICE_INTERFACE_DETAIL_DATA devItf_detail_data;
			devItf_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);
			devItf_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetail(dev_info, &devItf_data, devItf_detail_data, size, &size, NULL);

			_handle = CreateFile(devItf_detail_data->DevicePath, config.desired_access, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_OVERLAPPED, NULL);
			free(devItf_detail_data);

			if (INVALID_HANDLE_VALUE != _handle) {
				HIDD_ATTRIBUTES attributes;
				PHIDP_PREPARSED_DATA preparsed_data;
				HIDP_CAPS capabilities;
				HidD_GetAttributes(_handle, &attributes);
				HidD_GetPreparsedData(_handle, &preparsed_data);
				HidP_GetCaps(preparsed_data, &capabilities);
				HidD_FreePreparsedData(preparsed_data);

				if (attributes.VendorID == config.vendor_id &&
					attributes.ProductID == config.product_id &&
					capabilities.UsagePage == config.usage_page &&
					capabilities.Usage == config.usage) {

					COMMTIMEOUTS comm_time_outs;
					GetCommTimeouts(_handle, &comm_time_outs);
					comm_time_outs.ReadIntervalTimeout = 1;
					comm_time_outs.ReadTotalTimeoutConstant = 1;
					comm_time_outs.ReadTotalTimeoutMultiplier = 1;
					SetCommTimeouts(_handle, &comm_time_outs);

					SetFileCompletionNotificationModes(_handle, FILE_SKIP_COMPLETION_PORT_ON_SUCCESS | FILE_SKIP_SET_EVENT_ON_HANDLE);
				}
				else {
					CloseHandle(_handle);
					_handle = INVALID_HANDLE_VALUE;
				}
			}
		}
		SetupDiDestroyDeviceInfoList(dev_info);
	};
	inline ~device(void) noexcept {
		CloseHandle(_handle);
	};
public:
	inline void read(void* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		OVERLAPPED overlapped{};
		ReadFile(_handle, buffer, length, nullptr, &overlapped);
		//DWORD result;
		//while (!GetOverlappedResult(_handle, &overlapped, &result, false)) {
		//}
		while (STATUS_PENDING == ((volatile OVERLAPPED)overlapped).Internal) {
			_mm_pause();
			//_mm_mfence();
		}
	}
	inline void write(void const* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		OVERLAPPED overlapped{};
		WriteFile(_handle, buffer, length, nullptr, &overlapped);
		//DWORD result;
		//while (!GetOverlappedResult(_handle, &overlapped, &result, false)) {
		//}
		while (STATUS_PENDING == ((volatile OVERLAPPED)overlapped).Internal) {
			_mm_pause();
			//_mm_mfence();

		}
	}
private:
	HANDLE /*__restrict*/ _handle = INVALID_HANDLE_VALUE;
};