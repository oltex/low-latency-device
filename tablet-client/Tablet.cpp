#include "Tablet.h"

Tablet::Tablet(Device::Config const& devCfg, Config const config)
	: _device(devCfg), _config(config) {
	memset(_buf, 0, sizeof(_buf));
}

Tablet::~Tablet(void) {
}

int const Tablet::Read(void) noexcept {
	memset(_buf, 0, 1);
	_device.Read(_buf, _config.reportLen);

	if (_buf[0] != _config.reportId ||
		(_buf[1] & _config.detectMask) != _config.detectMask)
		return 0;
	return 1;
}