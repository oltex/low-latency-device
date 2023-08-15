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
	void Read(void* const buf, const int len) const noexcept;
	void Write( void* const buf, const int len) const noexcept;
private:
	HANDLE _handle = nullptr;
};