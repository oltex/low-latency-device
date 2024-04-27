#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config {
		BYTE const reportId, reportLen, detectMask;
	};
public:
	explicit Tablet(Device::Config const& devCfg, Config const config);
	~Tablet(void);
public:
	inline int const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	BYTE _buf[10];
};

inline int const Tablet::Read(void) noexcept {
	memset(_buf, 0, sizeof(_buf));
	_device.Read(_buf, _config.reportLen);

	if (_buf[0] != _config.reportId ||
		(_buf[1] & _config.detectMask) != _config.detectMask)
		return 0;
	return 1;
}