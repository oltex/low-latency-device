#pragma once
#include "Device.h"

class Tablet final {
public:
	struct Config final {
		int const reportId, reportLen, detectMask;
	};
	//struct Report {
	//	unsigned short x, y;
	//};
public:
	explicit Tablet(Device::Config const& devCfg, Config const& config) noexcept;
	~Tablet(void) noexcept;
public:
	bool const Read(void) noexcept;
private:
	Device const _device;
	Config const _config;
public:
	unsigned char _buf[10];
};