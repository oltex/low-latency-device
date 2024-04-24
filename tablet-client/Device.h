#pragma once
#include <Windows.h>
#include <SetupAPI.h>
#include <hidsdi.h>
#include <utility>

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