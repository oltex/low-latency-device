#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config {
		const BYTE reportId, reportLen, detectMask;
	};
	struct Report {
		UCHAR button;
		USHORT x, y;
	};
public:
	explicit Tablet(const Device::Config& devCfg, const Config config);
	~Tablet(void);
public:
	inline const int Read(void) noexcept;
public:
	Report _report;
private:
	const Device _device;
	const Config _config;
	BYTE _buf[10];
};

inline const int Tablet::Read(void) noexcept {
	memset(_buf, 0, 10);
	_device.Read(_buf, _config.reportLen);

	if (_buf[0] != _config.reportId)
		return 0;
	if ((_buf[1] & _config.detectMask) != _config.detectMask)
		return 0;

	_report.button = _buf[1];
	_report.x = _buf[2] | (_buf[3] << 8);
	_report.y = _buf[4] | (_buf[5] << 8);
	return 1;
}