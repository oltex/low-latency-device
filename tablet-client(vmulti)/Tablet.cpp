#include "Tablet.h"

Tablet::Tablet(const Device::Config& devCfg, const Config config)
	: _device(devCfg), _config(config) {
	memset(&_report, 0, sizeof(Report));
	memset(_buf, 0, 10);
}

Tablet::~Tablet(void) {
}

const int Tablet::Read(void) noexcept {
	memset(_buf, 0, 10);
	_device.Read(_buf, _config.reportLen);

	if (_buf[0] != _config.reportId ||
		(_buf[1] & _config.detectMask) != _config.detectMask)
		return 0;


	_report.button = _buf[1];
	_report.x = _buf[2] | (_buf[3] << 8);
	_report.y = _buf[4] | (_buf[5] << 8);
	return 1;
}