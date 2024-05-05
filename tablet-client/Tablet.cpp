#include "Tablet.h"

Tablet::Tablet(Device::Config const& devCfg, Config const& config) noexcept
	: _device(devCfg), _config(config) {
	memset(_buf, 0, sizeof(_buf));
}

Tablet::~Tablet(void) noexcept {
}