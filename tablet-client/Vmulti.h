#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Report final {
		BYTE const reportId = 0x02;
		//BYTE const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//BYTE const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
		UCHAR button = 0;
		USHORT x = 0, y = 0;
	};
public:
	explicit Vmulti(void);
	~Vmulti(void);
public:
	inline void Write(void) noexcept;
public:
	Report _report;
private:
	BYTE _buf[7]; //65
	Device const _device;
};

inline void Vmulti::Write(void) noexcept {
	memcpy(_buf, &_report, sizeof(Report));
	_device.Write(_buf, sizeof(_buf));
}