#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Report {
		const BYTE vmultiId = 0x40, reportLen = 9, reportId = 0x09; //hawku (len = 7 report = 3), otd (len = 9 report = 9)
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