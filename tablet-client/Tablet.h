#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config final {
		unsigned char const reportId, reportLen, detectMask;
	};
public:
	explicit Tablet(Device::Config const& devCfg, Config const& config) noexcept;
	~Tablet(void) noexcept;
public:
	inline bool const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	unsigned char _buf[10];
};
inline bool const Tablet::Read(void) noexcept {
	_device.Read(_buf, _config.reportLen);
	if (_buf[0] != _config.reportId ||
		(_buf[1] & _config.detectMask) != _config.detectMask)
		return false;
	return true;
}