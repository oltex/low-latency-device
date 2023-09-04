#include "Tablet.h"

Tablet::Tablet(const Device::Config& devCfg, const Config config)
	: _device(devCfg), _config(config) {
	memset(&_report, 0, sizeof(Report));
	memset(_buf, 0, 10);
}

Tablet::~Tablet(void) {
}