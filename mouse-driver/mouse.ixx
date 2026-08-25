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
export module mouse;
import std;
import <stdio.h>;
import <wchar.h>;

export class mouse final {
	struct report {
		unsigned char _button;
		unsigned char _padding;
		short _x, _y;
		signed char _wheel;
		signed char _pan;
	};
	HANDLE _handle;
	WINUSB_INTERFACE_HANDLE _winusb;
	unsigned char _pipe_id;

public:
	inline mouse(void) noexcept
		: _handle(INVALID_HANDLE_VALUE), _winusb(nullptr), _pipe_id(0) {
	}
	inline ~mouse(void) noexcept {
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
			{0x046d, 0xc088, L"VID_046D&PID_C088"}, //G Pro Wireless
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
				printf("USB: compatible mouse found:        VID %04X PID %04X\n", config._vendor_id, config._product_id);

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
				printf("WINUSB: interface bound:            MI_%02u {%08lX-...}\n", interface_descriptor.bInterfaceNumber, guid.Data1);

				USHORT max_packet_size = 0;
				for (UCHAR pipe_index = 0; pipe_index < interface_descriptor.bNumEndpoints; ++pipe_index) {
					WINUSB_PIPE_INFORMATION pipe_information;
					if (!WinUsb_QueryPipe(winusb, 0, pipe_index, &pipe_information))
						continue;
					if (pipe_information.PipeType == UsbdPipeTypeInterrupt && USB_ENDPOINT_DIRECTION_IN(pipe_information.PipeId)) {
						_pipe_id = pipe_information.PipeId;
						max_packet_size = pipe_information.MaximumPacketSize;
						goto found;
					}
				}
				WinUsb_Free(winusb);
				::CloseHandle(handle);
				continue;
			found:
				printf("WINUSB: pipe located:               0x%02X interrupt-in %huB\n", _pipe_id, max_packet_size);

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
		report _report;
		ULONG transferred;
		if (!::WinUsb_ReadPipe(_winusb, _pipe_id, reinterpret_cast<PUCHAR>(&_report), sizeof(_report), &transferred, nullptr)) {
			printf("DEVICE: connection lost:            waiting for device...\n");
			::WinUsb_Free(_winusb);
			::CloseHandle(_handle);
			_winusb = nullptr;
			_handle = INVALID_HANDLE_VALUE;
			return std::nullopt;
		}
		return _report;
	}
};
