#pragma once
#include "device.h"

class tablet final {
public:
	struct config final {
		unsigned char const report_id, report_length, detect_mask;
	};
public:
	inline explicit tablet(device::config const& devCfg, config const& config) noexcept
		: _device(devCfg), _config(config) {
		memset(_buffer, 0, sizeof(_buffer));
	}
	inline ~tablet(void) noexcept = default;
public:
	inline bool const read(void) noexcept {
		_device.read(_buffer, _config.report_length);
		if (_buffer[0] != _config.report_id || !(_buffer[1] & _config.detect_mask))
			return false;
		_buffer[0] = 0;
		return true;
	};
private:
	device const _device;
	config const _config;
public:
	unsigned char _buffer[10];
};