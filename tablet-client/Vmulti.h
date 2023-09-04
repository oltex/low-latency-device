#pragma once
#include "Device.h"

class Vmulti final {
public:
	struct Report {
		const BYTE reportId = 0x02;
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
	const Device _device;
	BYTE _buf[7];
};

inline void Vmulti::Write(void) noexcept {
	memcpy(_buf, &_report, sizeof(Report));
	_device.Write(_buf, 7);
}