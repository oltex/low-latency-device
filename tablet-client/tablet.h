#pragma once
#include "device.h"

class tablet final {
public:
	inline explicit tablet(void) noexcept
		: _device(0x056a, 0x037a, 0xFF0D, 0x0001),
		_report_id(0x02), _report_length(10), _detect_mask(0x40) {
		unsigned char buffer[2]{ 0x02, 0x02 };
		_device.set_feature(buffer, 2);
	}

	inline void const read(void) noexcept {
		_buffer[0] = 0;
		do
			_device.read(_buffer, _report_length);
		while (_buffer[0] != _report_id || !(_buffer[1] & _detect_mask));
	};
private:
	device const _device;
	unsigned char const _report_id;
	unsigned char const _report_length;
	unsigned char const _detect_mask;
public:
	unsigned char _buffer[10]{};
};

// ctl-470 : 0x056a, 0x00dd, 0x00D, 0x0001
// ctl-480 : 0x056A, 0x030E, 0xFF0D, 0x0001
// ctl-472 : 0x056a, 0x037a, 0xFF0D, 0x0001