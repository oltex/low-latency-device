#pragma once
#include <Windows.h>

class Device final {
public:
	struct Config final {
		unsigned long const desiredAccess;
		unsigned short const vendorId, productId;
		unsigned short const usagePage, usage;
	};
public:
	explicit Device(Config const& config) noexcept;
	~Device(void) noexcept;
public:
	inline void Read(void* const __restrict buf, unsigned char const len) const noexcept;
	inline void Write(void const* const __restrict buf, unsigned char const len) const noexcept;
private:
	HANDLE __restrict _handle = INVALID_HANDLE_VALUE;
};

inline void Device::Read(void* const __restrict buf, unsigned char const len) const noexcept {
	ReadFile(_handle, buf, len, NULL, NULL);
}

inline void Device::Write(void const* const __restrict buf, unsigned char const len) const noexcept {
	WriteFile(_handle, buf, len, NULL, NULL);
}