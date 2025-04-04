#pragma once
#include "device.h"

class tablet final {
public:
	inline explicit tablet(void) noexcept
		: _device(FILE_READ_DATA, 0x056a, 0x00dd, 0x00D, 0x0001),
		_report_id(0x02), _report_length(10), _detect_mask(0x40) {
		unsigned char buffer[2]{ 0x02, 0x02 };
		_device.set_feature(buffer, 2);
		_device.set_num_input_buffer(2);
	}

	inline bool const read(void) noexcept {
		_device.read(_buffer, _report_length);
		if (_buffer[0] != _report_id || !(_buffer[1] & _detect_mask))
			return false;
		_buffer[0] = 0;
		return true;
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