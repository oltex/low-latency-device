#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Report {
		const BYTE vmultiId = 0x40, reportLen = 7, reportId = 3;
		UCHAR button = 0;
		USHORT x = 0, y = 0;
	};
public:
	explicit Vmulti(void);
	~Vmulti(void);
public:
	void Write(void) noexcept;
public:
	Report _report;
private:
	const Device _device;
	BYTE _buf[65];
};