#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Config final {
		//unsigned char const reportId = 0x02;
		unsigned char const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//unsigned char const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
	};
public:
	explicit Vmulti(void) noexcept;
	~Vmulti(void) noexcept;
public:
	inline void Write(void) noexcept;
private:
	Device const _device;
public:
	unsigned char _buf[65]; //7, 65
};

inline void Vmulti::Write(void) noexcept {
	//memcpy(_buf + 4, &_report, sizeof(Report));
	_device.Write(_buf, sizeof(_buf));
}