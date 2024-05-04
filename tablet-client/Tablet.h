#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config {
		unsigned char const reportId, reportLen, detectMask;
	};
public:
	explicit Tablet(Device::Config const& devCfg, Config const& config);
	~Tablet(void);
public:
	bool const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	unsigned char _buf[10];
};