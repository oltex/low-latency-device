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
	void Read(void* const /*__restrict*/ buf, int const len) const noexcept;
	void Write(void const* const /*__restrict*/ buf, int const len) const noexcept;
private:
	HANDLE /*__restrict*/ _handle = INVALID_HANDLE_VALUE;
};