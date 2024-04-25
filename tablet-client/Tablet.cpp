#include "Tablet.h"

Tablet::Tablet(Device::Config const& devCfg, Config const config)
	: _device(devCfg), _config(config) {
	memset(&_report, 0, sizeof(Report));
	memset(_buf, 0, sizeof(_buf));
}

Tablet::~Tablet(void) {
}