module;
#include <Windows.h>
#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>
#pragma comment(lib, "ntdll.lib")
#include <winternl.h>
export module tablet;
import std;
import <stdio.h>;

using NtReadFile = NTSTATUS(WINAPI*)(
	HANDLE FileHandle, HANDLE Event,
	PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);

export class tablet final {
	struct report {
		unsigned char _button;
		unsigned char _mask;
		unsigned short _x, _y;
		unsigned short _pressure;
		unsigned short _padding;
	};
	NtReadFile _nt_read_file;
	HANDLE /*__restrict*/ _handle;
public:
	inline tablet(void) noexcept
		: _nt_read_file(reinterpret_cast<NtReadFile>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "NtReadFile"))) {
	}
	inline ~tablet(void) noexcept {
		::CloseHandle(_handle);
	}

	inline auto connect(void) noexcept -> bool {
		static constexpr struct {
			unsigned short const _vendor_id, _product_id, _usage_page, _usage;
		} _config[] = {
			{0x056a, 0x00dd, 0x000d, 0x0001}, //470
			{0x056a, 0x037a, 0xff0d, 0x0001}, //472
			{0x056a, 0x030e, 0xff0d, 0x0001}, //480
		};
		GUID guid;
		HidD_GetHidGuid(&guid);
		auto const info = SetupDiGetClassDevsW(&guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

		for (auto index = 0ul;; ++index) {
			SP_DEVINFO_DATA data;
			data.cbSize = sizeof(SP_DEVINFO_DATA);
			if (!SetupDiEnumDeviceInfo(info, index, &data))
				break;
			WCHAR path[512];
			SetupDiGetDeviceRegistryPropertyW(info, &data, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, nullptr, reinterpret_cast<PBYTE>(path), sizeof(path), nullptr);

			UNICODE_STRING string;
			RtlInitUnicodeString(&string, path);
			OBJECT_ATTRIBUTES attribute;
			InitializeObjectAttributes(&attribute, &string, OBJ_CASE_INSENSITIVE, nullptr, nullptr);

			IO_STATUS_BLOCK block;
			if (0 != NtCreateFile(&_handle, FILE_READ_DATA | SYNCHRONIZE, &attribute, &block, nullptr, 0, 0, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT /*| FILE_SEQUENTIAL_ONLY| FILE_NO_INTERMEDIATE_BUFFERING*/, nullptr, 0))
				continue;
			HIDD_ATTRIBUTES attribute2;
			PHIDP_PREPARSED_DATA preparsed_data;
			HIDP_CAPS capability;
			HidD_GetAttributes(_handle, &attribute2);
			HidD_GetPreparsedData(_handle, &preparsed_data);
			HidP_GetCaps(preparsed_data, &capability);
			HidD_FreePreparsedData(preparsed_data);

			for (auto const& config : _config) {
				if (attribute2.VendorID != config._vendor_id || attribute2.ProductID != config._product_id || capability.UsagePage != config._usage_page || capability.Usage != config._usage)
					continue;

				printf("Found Compatible Tablet.\n"\
					" VendorID: 0x%04x\n"\
					" ProductID: 0x%04x\n"\
					" UsagePage: 0x%04x\n"\
					" Usage: 0x%04x\n\n"
					, attribute2.VendorID, attribute2.ProductID
					, capability.UsagePage, capability.Usage);
				unsigned char buffer[]{0x02, 0x02};
				HidD_SetFeature(_handle, buffer, sizeof(buffer));
				SetupDiDestroyDeviceInfoList(info);
				return true;
			}
			CloseHandle(_handle);
		}
		SetupDiDestroyDeviceInfoList(info);
		return false;
	}
	inline auto read(void) noexcept -> std::optional<report> {
		static constexpr unsigned char _report_id = 0x02, _report_length = 10, _detect_mask = 0x40;
		report _report;
		do {
			IO_STATUS_BLOCK block;
			if (0 > _nt_read_file(_handle, nullptr, nullptr, nullptr, &block, &_report, _report_length, nullptr, nullptr)) 
				return std::nullopt;
		} while (_report._button != _report_id || !(_report._mask & _detect_mask));
		return _report;
	};
};

//NtOpenFile(&_handle, FILE_READ_DATA | SYNCHRONIZE, &attribute, &block, 0, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
//HidD_SetNumInputBuffers(_handle, 512);