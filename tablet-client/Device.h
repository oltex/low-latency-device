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
	void Read(void* const __restrict buf, const int len) const noexcept;
	void Write(const void* const __restrict buf, const int len) const noexcept;
private:
	HANDLE __restrict _handle = nullptr;
};