#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config {
		const BYTE reportId, reportLen, detectMask;
	};
	struct Report {
		BYTE id;
		UCHAR button;
		USHORT x, y;
		USHORT pressure;
	};
public:
	explicit Tablet(const Device::Config& deviceCfg, const Config& tabletCfg);
	~Tablet(void);
public:
	int Read(void) noexcept;
public:
	const Config _config;
	Report _report;
private:
	Device* _device = nullptr;
	UCHAR _buf[10];
};