#pragma once
#include <Windows.h>
#include <SetupAPI.h>
#include <hidsdi.h>

class Device final {
public:
	struct Config {
		const USHORT vendorId, productId, usagePage, usage;
	};
public:
	explicit Device(const Config& config);
	~Device(void);
public:
	int Read(void* const buf, const BYTE& len) noexcept;
	int	Write(const void* const buf, const BYTE& len) noexcept;
private:
	HANDLE _handle = nullptr;
};