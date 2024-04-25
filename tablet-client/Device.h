#pragma once
#include <Windows.h>
#include <SetupAPI.h>
#include <hidsdi.h>

class Device final {
public:
	struct Config final {
		USHORT const vendorId, productId;
		USAGE const usagePage, usage;
		DWORD const desiredAccess;
	};
public:
	explicit Device(Config const& config);
	~Device(void);
public:
	inline void Read(void* const __restrict buf, int const len) const noexcept;
	inline void Write(void const* const __restrict buf, int const len) const noexcept;
private:
	HANDLE __restrict _handle = INVALID_HANDLE_VALUE;
};

inline void Device::Read(void* const __restrict buf, int const len) const noexcept {
	ReadFile(_handle, buf, len, NULL, NULL);
}

inline void Device::Write(void const* const __restrict buf, int const len) const noexcept {
	WriteFile(_handle, buf, len, NULL, NULL);
}