#pragma once
#include "device.h"

class tablet final {
public:
	struct config final {
		unsigned char const reportId, reportLen, detectMask;
	};
public:
	inline explicit tablet(device::config const& devCfg, config const& config) noexcept
		: _device(devCfg), _config(config) {
		memset(_buf, 0, sizeof(_buf));
	}
	inline ~tablet(void) noexcept = default;
public:
	inline bool const read(void) noexcept {
		_device.read(_buf, _config.reportLen);
		if (_buf[0] != _config.reportId || !(_buf[1] & _config.detectMask))
			return false;
		_buf[0] = 0;
		return true;
	};
private:
	device const _device;
	config const _config;
public:
	unsigned char _buf[10];
};