#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config final {
		unsigned char const reportId, reportLen, detectMask;
	};
public:
	inline explicit Tablet(Device::Config const& devCfg, Config const& config) noexcept
		: _device(devCfg), _config(config) {
		memset(_buf, 0, sizeof(_buf));
	}
	inline ~Tablet(void) noexcept = default;
public:
	inline bool const Read(void) noexcept {
		_device.Read(_buf, _config.reportLen);
		if (_buf[0] != _config.reportId || !(_buf[1] & _config.detectMask))
			return false;
		_buf[0] = 0;
		return true;
	};
private:
	Device const _device;
	Config const _config;
public:
	unsigned char _buf[10];
};