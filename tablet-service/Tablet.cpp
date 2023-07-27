#include "Tablet.h"

Tablet::Tablet(const Device::Config& deviceCfg, const Config& tabletCfg)
	: _config(tabletCfg) {
	_device = new Device(deviceCfg);
	memset(&_report, 0, sizeof(Report));
	memset(_buf, 0, 10);
}

Tablet::~Tablet(void) {
	delete _device;
}

int Tablet::Read(void) noexcept {
	memset(_buf, 0, 10);
	if (!_device->Read(_buf, _config.reportLen))
		return 0;

	_report.id = _buf[0];
	if (_report.id != _config.reportId)
		return 0;
	_report.button = _buf[1];
	if ((_report.button & _config.detectMask) != _config.detectMask)
		return 0;

	_report.x = _buf[2] | (_buf[3] << 8);
	_report.y = _buf[4] | (_buf[5] << 8);
	_report.pressure = _buf[6] | (_buf[7] << 8);
	return 1;
}