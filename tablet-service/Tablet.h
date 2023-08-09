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
	const int Read(void) noexcept;
public:
	Report _report;
private:
	const Device _device;
	const Config _config;
	BYTE _buf[10];
};