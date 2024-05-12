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
	void Read(void* const /*__restrict*/ buf, unsigned int const len) const noexcept;
	void Write(void const* const /*__restrict*/ buf, unsigned int const len) const noexcept;
private:
	HANDLE /*__restrict*/ _handle = INVALID_HANDLE_VALUE;
};