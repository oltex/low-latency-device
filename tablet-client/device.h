#pragma once
#include <Windows.h>
#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>

class device final {
public:
	inline explicit device(unsigned long const desired_access, unsigned short const vendor_id, unsigned short const product_id, unsigned short const usage_page, unsigned short const usage) noexcept {
		GUID guid;
		HidD_GetHidGuid(&guid);
		HDEVINFO info = SetupDiGetClassDevsW(&guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

		for (unsigned long index = 0;; ++index) {
			SP_DEVICE_INTERFACE_DATA interface_data;
			interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			SetupDiEnumDeviceInterfaces(info, nullptr, &guid, index, &interface_data);

			unsigned long size;
			SetupDiGetDeviceInterfaceDetailW(info, &interface_data, nullptr, 0, &size, nullptr);
			PSP_DEVICE_INTERFACE_DETAIL_DATA detail_data = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(size));
			detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetailW(info, &interface_data, detail_data, size, &size, nullptr);

			_handle = CreateFileW(detail_data->DevicePath, desired_access, 0, nullptr, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, nullptr);
			free(detail_data);
			if (INVALID_HANDLE_VALUE != _handle) {
				HIDD_ATTRIBUTES attributes;
				PHIDP_PREPARSED_DATA preparsed_data;
				HIDP_CAPS capabilities;
				HidD_GetAttributes(_handle, &attributes);
				HidD_GetPreparsedData(_handle, &preparsed_data);
				HidP_GetCaps(preparsed_data, &capabilities);
				HidD_FreePreparsedData(preparsed_data);

				if (attributes.VendorID == vendor_id && attributes.ProductID == product_id && capabilities.UsagePage == usage_page && capabilities.Usage == usage)
					break;
				CloseHandle(_handle);
			}
		}
		SetupDiDestroyDeviceInfoList(info);
	};
	inline ~device(void) noexcept {
		CloseHandle(_handle);
	};

	inline void read(void* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		ReadFile(_handle, buffer, length, nullptr, nullptr);
	}
	inline void write(void const* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		WriteFile(_handle, buffer, length, nullptr, nullptr);
	}
	inline void set_feature(void* const /*__restrict*/ buffer, unsigned char const length) const noexcept {
		HidD_SetFeature(_handle, buffer, length);
	}
	inline void set_num_input_buffer(unsigned long number) const noexcept {
		HidD_SetNumInputBuffers(_handle, number);
	}
private:
	HANDLE /*__restrict*/ _handle;
};

//COMMTIMEOUTS comm_time_outs;
//GetCommTimeouts(_handle, &comm_time_outs);
//comm_time_outs.ReadIntervalTimeout = 1;
//comm_time_outs.ReadTotalTimeoutConstant = 1;
//comm_time_outs.ReadTotalTimeoutMultiplier = 1;
//SetCommTimeouts(_handle, &comm_time_outs);

//_mm_clflush(&overlapped.Internal);
