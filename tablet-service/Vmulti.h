#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Report {
		const BYTE vmultiId = 0x40, reportLen = 7, reportId = 3;
		UCHAR button;
		USHORT x, y;
		BYTE wheel;
	};
public:
	explicit Vmulti(void);
	~Vmulti(void);
public:
	int Write(void) noexcept;
public:
	Report _report;
private:
	Device* _device = nullptr;
	BYTE _buf[65];
};