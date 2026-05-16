#pragma once
#include <Windows.h>
#pragma comment(lib, "hid.lib")
#include <hidsdi.h>
#pragma comment(lib, "setupapi.lib")
#include <SetupAPI.h>
#pragma comment(lib, "ntdll.lib")
#include <winternl.h>

using NtReadFile = NTSTATUS(WINAPI*)(
	HANDLE FileHandle, HANDLE Event,
	PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);

class tablet final {
public:
	NtReadFile _nt_read_file;
	LARGE_INTEGER offset{ 0 };
	HANDLE /*__restrict*/ _handle;
	alignas(2) unsigned char _buffer[10]{};
	inline static constexpr unsigned char _report_id = 0x02;
	inline static constexpr unsigned char _report_length = 10;
	inline static constexpr unsigned char _detect_mask = 0x40;

	inline explicit tablet(void) noexcept
		: _nt_read_file(reinterpret_cast<NtReadFile>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtReadFile"))) {
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

		for (auto index = 0lu;; ++index) {
			SP_DEVINFO_DATA data;
			data.cbSize = sizeof(SP_DEVINFO_DATA);
			SetupDiEnumDeviceInfo(info, index, &data);
			WCHAR path[512];
			SetupDiGetDeviceRegistryPropertyW(info, &data, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, nullptr, reinterpret_cast<PBYTE>(path), sizeof(path), nullptr);

			UNICODE_STRING string;
			RtlInitUnicodeString(&string, path);
			OBJECT_ATTRIBUTES attribute;
			InitializeObjectAttributes(&attribute, &string, OBJ_CASE_INSENSITIVE, nullptr, nullptr);

			IO_STATUS_BLOCK block;
			if (0 == NtCreateFile(&_handle, FILE_READ_DATA | SYNCHRONIZE, &attribute, &block, nullptr, 0, 0, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT /*| FILE_SEQUENTIAL_ONLY| FILE_NO_INTERMEDIATE_BUFFERING*/, nullptr, 0)) {
				HIDD_ATTRIBUTES attribute;
				PHIDP_PREPARSED_DATA preparsed_data;
				HIDP_CAPS capability;
				HidD_GetAttributes(_handle, &attribute);
				HidD_GetPreparsedData(_handle, &preparsed_data);
				HidP_GetCaps(preparsed_data, &capability);
				HidD_FreePreparsedData(preparsed_data);

				for (auto index = 0lu; index < 3; ++index) {
					if (attribute.VendorID == config[index]._vendor_id &&
						attribute.ProductID == config[index]._product_id &&
						capability.UsagePage == config[index]._usage_page &&
						capability.Usage == config[index]._usage) {

						unsigned char buffer[]{ 0x02, 0x02 };
						HidD_SetFeature(_handle, buffer, sizeof(buffer));

						SetupDiDestroyDeviceInfoList(info);
						return;
					}
				}
				CloseHandle(_handle);
			}
		}
	}

	inline void const read(void) noexcept {
		IO_STATUS_BLOCK block;
		do
			_nt_read_file(_handle, nullptr, nullptr, nullptr, &block, _buffer, _report_length, &offset, nullptr);
		while (_buffer[0] != _report_id || !(_buffer[1] & _detect_mask));
	};
};

//_event = CreateEventW(nullptr, false, false, nullptr);
//HANDLE /*__restrict*/ _event;
//OVERLAPPED overlapped{};
//overlapped.hEvent = _event;
//ReadFile(_handle, _buffer, _report_length, nullptr, &overlapped);
//WaitForSingleObject(_event, INFINITE);
//while (STATUS_PENDING == reinterpret_cast<volatile OVERLAPPED*>(&overlapped)->Internal) {
//}

//for (unsigned long index = 0;; ++index) {
//	SP_DEVICE_INTERFACE_DATA interface_data;
//	interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
//	SetupDiEnumDeviceInterfaces(info, nullptr, &guid, index, &interface_data);
//	unsigned long size;
//	SetupDiGetDeviceInterfaceDetailW(info, &interface_data, nullptr, 0, &size, nullptr);
//	PSP_DEVICE_INTERFACE_DETAIL_DATA_W detail_data = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(malloc(size));
//	detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
//	SetupDiGetDeviceInterfaceDetailW(info, &interface_data, detail_data, size, &size, nullptr);
//
//	_handle = CreateFileW(detail_data->DevicePath, FILE_READ_DATA, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_DEVICE | FILE_FLAG_OVERLAPPED /* | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH*/, nullptr);
//	free(detail_data);
//	if (INVALID_HANDLE_VALUE != _handle) {
//		HIDD_ATTRIBUTES attribute;
//		PHIDP_PREPARSED_DATA preparsed_data;
//		HIDP_CAPS capability;
//		HidD_GetAttributes(_handle, &attribute);
//		HidD_GetPreparsedData(_handle, &preparsed_data);
//		HidP_GetCaps(preparsed_data, &capability);
//		HidD_FreePreparsedData(preparsed_data);
//
//		for (unsigned long index = 0; index < 3; ++index) {
//			if (attribute.VendorID == config[index]._vendor_id &&
//				attribute.ProductID == config[index]._product_id &&
//				capability.UsagePage == config[index]._usage_page &&
//				capability.Usage == config[index]._usage) {
//				goto exit;
//			}
//		}
//		CloseHandle(_handle);
//	}
//}

//NtOpenFile(&_handle, FILE_READ_DATA | SYNCHRONIZE, &attribute, &block, 0, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
//
//HidD_SetNumInputBuffers(_handle, 512);