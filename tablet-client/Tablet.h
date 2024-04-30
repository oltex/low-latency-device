#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config {
		int const reportId, reportLen, detectMask;
	};
public:
	explicit Tablet(Device::Config const& devCfg, Config const& config);
	~Tablet(void);
public:
	int const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	BYTE _buf[10];
};