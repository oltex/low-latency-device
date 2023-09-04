#pragma once
#include <Windows.h>
#include <SetupAPI.h>
#include <hidsdi.h>

class Device final {
public:
	struct Config {
		const USHORT vendorId, productId;
		const USAGE usagePage, usage;
		const DWORD desiredAccess;
	};
public:
	explicit Device(const Config& config);
	~Device(void);
public:
	inline void Read(void* const __restrict buf, const int len) const noexcept;
	inline void Write(const void* const __restrict buf, const int len) const noexcept;
private:
	HANDLE __restrict _handle = nullptr;
};

inline void Device::Read(void* const __restrict buf, const int len) const noexcept {
	ReadFile(_handle, buf, len, NULL, NULL);
}

inline void Device::Write(const void* const __restrict buf, const int len) const noexcept {
	WriteFile(_handle, buf, len, NULL, NULL);
}