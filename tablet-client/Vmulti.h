#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Config final {
		//unsigned char const reportId = 0x02;
		unsigned char const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//unsigned char const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
	};
public:
	inline explicit Vmulti(void) noexcept
		//: _device(Device::Config{ FILE_WRITE_DATA, 0x00EE, 0xBACC, 0xFF00, 0x0001 }) {
		: _device(Device::Config{ FILE_WRITE_DATA, 0x00FF, 0xBACC, 0xFF00, 0x0001 }) { //hawku
		Config config;
		memcpy(_buf, &config, sizeof(Config));
	}
	inline ~Vmulti(void) noexcept = default;
public:
	inline void Write(void) const noexcept {
		_device.Write(_buf, sizeof(_buf));
	}
private:
	Device const _device;
public:
	unsigned char _buf[65]{}; //7, 65
};