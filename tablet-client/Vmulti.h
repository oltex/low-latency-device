#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Config {
		//BYTE const reportId = 0x02;
		BYTE const vmultiId = 0x40, reportLen = 7, reportId = 3; //hawku
		//BYTE const vmultiId = 0x40, reportLen = 9, reportId = 9; //otd
	};
	//struct Report {
	//	USHORT x = 0, y = 0;
	//};
public:
	explicit Vmulti(void);
	~Vmulti(void);
public:
	void Write(void) noexcept;
private:
	Device const _device;
public:
	//Report _report;
	BYTE _buf[65]; //7, 65
};