#pragma once
#include "device.h"
class vmulti final {
public:
	struct config final {
		unsigned char const vmulti_id = 0x40, report_length = 7, report_id = 3; //hawku
	};
public:
	inline explicit vmulti(void) noexcept
		: _device(device::config{ FILE_WRITE_DATA, 0x00FF, 0xBACC, 0xFF00, 0x0001 }) { //hawku
		config config;
		memcpy(_buffer, &config, sizeof(config));
	}
	inline ~vmulti(void) noexcept = default;
public:
	inline void write(void) const noexcept {
		_device.write(_buffer, sizeof(_buffer));
	}
private:
	device const _device;
public:
	unsigned char _buffer[65]{}; //7, 65
};

// struct config final {
//  unsigned char const report_id = 0x02;
//  unsigned char const vmulti_id = 0x40, report_length = 9, report_id = 9; //otd
// };
// : _device(device::config{ FILE_WRITE_DATA, 0x00EE, 0xBACC, 0xFF00, 0x0001 }) {