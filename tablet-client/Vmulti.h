#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Config {
		//BYTE const reportId = 0x02;
		BYTE const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//BYTE const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
	};
public:
	explicit Vmulti(void);
	~Vmulti(void);
public:
	inline void Write(void) noexcept;
private:
	Device const _device;
public:
	BYTE _buf[65]; //7, 65
};

inline void Vmulti::Write(void) noexcept {
	_device.Write(_buf, sizeof(_buf));
}