#pragma once
#include <Windows.h>
#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>

class tablet final {
public:
	inline explicit tablet(void) noexcept {
		static constexpr struct {
			unsigned short const _vendor_id, _product_id, _usage_page, _usage;
		} config[] = {
			{ 0x056a, 0x00dd, 0x000d, 0x0001 }, //470
			{ 0x056a, 0x037a, 0xff0d, 0x0001 }, //472
			{ 0x056a, 0x030e, 0xff0d, 0x0001 }, //480
		};
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

			_handle = CreateFileW(detail_data->DevicePath, FILE_READ_DATA, 0, nullptr, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, nullptr);
			free(detail_data);
			if (INVALID_HANDLE_VALUE != _handle) {
				HIDD_ATTRIBUTES attributes;
				PHIDP_PREPARSED_DATA preparsed_data;
				HIDP_CAPS capabilities;
				HidD_GetAttributes(_handle, &attributes);
				HidD_GetPreparsedData(_handle, &preparsed_data);
				HidP_GetCaps(preparsed_data, &capabilities);
				HidD_FreePreparsedData(preparsed_data);

				for (int index = 0; index < 3; ++index) {
					if (attributes.VendorID == config[index]._vendor_id &&
						attributes.ProductID == config[index]._product_id &&
						capabilities.UsagePage == config[index]._usage_page &&
						capabilities.Usage == config[index]._usage) {
						goto exit;
					}
				}
				CloseHandle(_handle);
			}
		}
	exit:
		SetupDiDestroyDeviceInfoList(info);

		unsigned char buffer[2]{ 0x02, 0x02 };
		HidD_SetFeature(_handle, buffer, 2);
		HidD_SetNumInputBuffers(_handle, 2);
	}

	inline void const read(void) noexcept {
		_buffer[0] = 0;
		do
			ReadFile(_handle, _buffer, _report_length, nullptr, nullptr);
		while (_buffer[0] != _report_id || !(_buffer[1] & _detect_mask));
	};

	HANDLE /*__restrict*/ _handle;
	alignas(2) unsigned char _buffer[10]{};
	inline static constexpr unsigned char  _report_id = 0x02;
	inline static constexpr unsigned char const _report_length = 10;
	inline static constexpr unsigned char const _detect_mask = 0x40;
};

// ctl-470 : 0x056a, 0x00dd, 0x00D, 0x0001
// ctl-480 : 0x056A, 0x030E, 0xFF0D, 0x0001
// ctl-472 : 0x056a, 0x037a, 0xFF0D, 0x0001


//COMMTIMEOUTS comm_time_outs;
//GetCommTimeouts(_handle, &comm_time_outs);
//comm_time_outs.ReadIntervalTimeout = 1;
//comm_time_outs.ReadTotalTimeoutConstant = 1;
//comm_time_outs.ReadTotalTimeoutMultiplier = 1;
//SetCommTimeouts(_handle, &comm_time_outs);

//_mm_clflush(&overlapped.Internal);