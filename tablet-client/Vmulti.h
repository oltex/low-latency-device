#pragma once
#include "device.h"

class vmulti final {
public:
	struct config final {
		//unsigned char const reportId = 0x02;
		unsigned char const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//unsigned char const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
	};
public:
	inline explicit vmulti(void) noexcept
		//: _device(Device::Config{ FILE_WRITE_DATA, 0x00EE, 0xBACC, 0xFF00, 0x0001 }) {
		: _device(device::config{ FILE_WRITE_DATA, 0x00FF, 0xBACC, 0xFF00, 0x0001 }) { //hawku
		config config;
		memcpy(_buf, &config, sizeof(config));
	}
	inline ~vmulti(void) noexcept = default;
public:
	inline void write(void) const noexcept {
		_device.write(_buf, sizeof(_buf));
	}
private:
	device const _device;
public:
	unsigned char _buf[65]{}; //7, 65
};