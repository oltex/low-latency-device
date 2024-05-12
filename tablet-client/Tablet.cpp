#include "Tablet.h"

Tablet::Tablet(Device::Config const& devCfg, Config const& config) noexcept
	: _device(devCfg), _config(config) {
	memset(_buf, 0, sizeof(_buf));
}

Tablet::~Tablet(void) noexcept {
}

int const Tablet::Read(void) noexcept {
	_device.Read(_buf, _config.reportLen);
	if (_buf[0] != _config.reportId ||
		(_buf[1] & _config.detectMask) != _config.detectMask)
		return false;
	_buf[0] = 0;
	return true;
}