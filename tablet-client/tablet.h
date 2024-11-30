#pragma once
#include "device.h"
#include <iostream>
class tablet final {
public:
	struct config final {
		unsigned char const report_id, report_length, detect_mask;
	};
public:
	inline explicit tablet(void) noexcept
		: _device(device::config{ FILE_READ_DATA, 0x056a, 0x00dd, 0x00D, 0x0001 /*0x056A, 0x030E, 0xFF0D, 0x0001*/ }), _config(tablet::config{ 0x02, 10, 0x40 }) {
		unsigned char buffer[2]{ 0x02, 0x02 };
		_device.set_feature(buffer, 2);
	}
	inline ~tablet(void) noexcept = default;
public:
	inline bool const read(void) noexcept {
		_device.read(_buffer, _config.report_length);
		std::cout << "me" << std::endl;
		if (_buffer[0] != _config.report_id || !(_buffer[1] & _config.detect_mask))
			return false;
		_buffer[0] = 0;
		return true;
	};
private:
	device const _device;
	config const _config;
public:
	unsigned char _buffer[10]{};
};