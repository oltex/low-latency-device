#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config final {
		unsigned int const reportId, reportLen, detectMask;
	};
public:
	explicit Tablet(Device::Config const& devCfg, Config const& config) noexcept;
	~Tablet(void) noexcept;
public:
	int const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	unsigned char _buf[10];
};