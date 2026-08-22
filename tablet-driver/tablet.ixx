module;
#include <Windows.h>
#pragma comment(lib, "winusb.lib")
#include <winusb.h>
#include <usb.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>
#pragma comment(lib, "rpcrt4.lib")
#include <rpc.h>
#pragma comment(lib, "advapi32.lib")
export module tablet;
import std;
import <stdio.h>;
import <wchar.h>;

export class tablet final {
	struct report {
		unsigned char _button;
		unsigned char _mask;
		unsigned short _x, _y;
		unsigned short _pressure;
		unsigned short _padding;
	};
	HANDLE _handle;
	WINUSB_INTERFACE_HANDLE _winusb;
	unsigned char _pipe_id;

public:
	inline tablet(void) noexcept
		: _handle(INVALID_HANDLE_VALUE), _winusb(nullptr), _pipe_id(0) {
	}
	inline ~tablet(void) noexcept {
		if (_winusb)
			WinUsb_Free(_winusb);
		if (INVALID_HANDLE_VALUE != _handle)
			::CloseHandle(_handle);
	}

	inline auto connect(void) noexcept -> bool {
		static constexpr struct {
			unsigned short const _vendor_id, _product_id;
			WCHAR const* _hardware_id;
		} _config[]{
			{0x056a, 0x00dd, L"VID_056A&PID_00DD"}, //470
			{0x056a, 0x037a, L"VID_056A&PID_037A"}, //472
			{0x056a, 0x030e, L"VID_056A&PID_030E"}, //480
		};

		auto const info = SetupDiGetClassDevsW(nullptr, L"USB", nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
		if (INVALID_HANDLE_VALUE == info)
			return false;
		for (auto index = 0ul;; ++index) {
			SP_DEVINFO_DATA data;
			data.cbSize = sizeof(SP_DEVINFO_DATA);
			if (!SetupDiEnumDeviceInfo(info, index, &data))
				break;

			WCHAR property[512]{};
			if (!SetupDiGetDeviceRegistryPropertyW(info, &data, SPDRP_HARDWAREID, nullptr, reinterpret_cast<PBYTE>(property), sizeof(property), nullptr))
				continue;

			for (auto const& config : _config) {
				if (!wcsstr(property, config._hardware_id))
					continue;
				printf("Found Compatible Tablet.\n"\
					" VendorID: 0x%04x\n"\
					" ProductID: 0x%04x\n"
					, config._vendor_id, config._product_id);

				//guid
				auto const key = SetupDiOpenDevRegKey(info, &data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
				if (INVALID_HANDLE_VALUE == key)
					continue;
				WCHAR string[64]{};
				DWORD size = sizeof(string);
				auto const result = RegQueryValueExW(key, L"DeviceInterfaceGUIDs", nullptr, nullptr, reinterpret_cast<LPBYTE>(string), &size);
				RegCloseKey(key);
				if (ERROR_SUCCESS != result)
					continue;
				string[wcslen(string) - 1] = L'\0';
				GUID guid;
				if (RPC_S_OK != UuidFromStringW(reinterpret_cast<RPC_WSTR>(string + 1), &guid))
					continue;

				auto const interface_info = SetupDiGetClassDevsW(&guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
				if (INVALID_HANDLE_VALUE == interface_info)
					continue;

				//file
				SP_DEVICE_INTERFACE_DATA interface_data;
				interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
				if (!SetupDiEnumDeviceInterfaces(interface_info, nullptr, &guid, 0, &interface_data)) {
					SetupDiDestroyDeviceInfoList(interface_info);
					continue;
				}
				DWORD required;
				SetupDiGetDeviceInterfaceDetailW(interface_info, &interface_data, nullptr, 0, &required, nullptr);
				auto const buffer = std::make_unique<unsigned char[]>(required);
				auto const detail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(buffer.get());
				detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
				if (!SetupDiGetDeviceInterfaceDetailW(interface_info, &interface_data, detail, required, nullptr, nullptr)) {
					SetupDiDestroyDeviceInfoList(interface_info);
					continue;
				}
				auto const handle = ::CreateFileW(detail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
				SetupDiDestroyDeviceInfoList(interface_info);
				if (INVALID_HANDLE_VALUE == handle)
					continue;
				WINUSB_INTERFACE_HANDLE winusb;
				if (!WinUsb_Initialize(handle, &winusb)) {
					::CloseHandle(handle);
					continue;
				}

				//pipe
				USB_INTERFACE_DESCRIPTOR interface_descriptor;
				if (!WinUsb_QueryInterfaceSettings(winusb, 0, &interface_descriptor)) {
					WinUsb_Free(winusb);
					::CloseHandle(handle);
					continue;
				}
				for (UCHAR pipe_index = 0; pipe_index < interface_descriptor.bNumEndpoints; ++pipe_index) {
					WINUSB_PIPE_INFORMATION pipe_information;
					if (!WinUsb_QueryPipe(winusb, 0, pipe_index, &pipe_information))
						continue;
					if (pipe_information.PipeType == UsbdPipeTypeInterrupt && USB_ENDPOINT_DIRECTION_IN(pipe_information.PipeId)) {
						_pipe_id = pipe_information.PipeId;
						goto found;
					}
				}
				WinUsb_Free(winusb);
				::CloseHandle(handle);
				continue;
			found:
				printf("Configuring WinUSB Settings.\n");
				unsigned char report_data[]{0x02, 0x02};
				WINUSB_SETUP_PACKET const packet{
					.RequestType = 0x21,
					.Request = 0x09,
					.Value = 0x0302,
					.Index = interface_descriptor.bInterfaceNumber,
					.Length = sizeof(report_data)};
				ULONG feature_transferred;
				if (WinUsb_ControlTransfer(winusb, packet, report_data, sizeof(report_data), &feature_transferred, nullptr))
					printf(" Feature: (0x2, 0x2)\n\n");

				_handle = handle;
				_winusb = winusb;
				SetupDiDestroyDeviceInfoList(info);
				return true;
			}
		}
		SetupDiDestroyDeviceInfoList(info);
		return false;
	}
	inline auto read(void) noexcept -> std::optional<report> {
		static constexpr unsigned char _report_id = 0x02, _report_length = 10, _detect_mask = 0x40;
		report _report;
		do {
			ULONG transferred;
			if (!::WinUsb_ReadPipe(_winusb, _pipe_id, reinterpret_cast<PUCHAR>(&_report), _report_length, &transferred, nullptr)) {
				::WinUsb_Free(_winusb);
				::CloseHandle(_handle);
				_winusb = nullptr;
				_handle = INVALID_HANDLE_VALUE;
				return std::nullopt;
			}
		} while (_report._button != _report_id || !(_report._mask & _detect_mask));
		return _report;
	};
};