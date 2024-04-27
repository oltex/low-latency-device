#include "Client.h"

Client::Client(Setting const& setting)
	: _tablet(Device::Config{ 0x056A, 0x030E, 0xFF0D, 0x0001, FILE_READ_DATA }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) {
}

#include<iostream>
void Client::Run(void) noexcept {
	register const int left = _setting._area._left, right = _setting._area._right, top = _setting._area._top, bottom = _setting._area._bottom;
	register const float width = _setting._area._width, height = _setting._area._height;

	for (;;) {
		if (!_tablet.Read())
			continue;

		__int64 tsc = __rdtsc();

		_vmulti._buf[3] = _tablet._buf[1] & 0x7;

		register int x = _tablet._buf[2] | (_tablet._buf[3] << 8);
		register int y = _tablet._buf[4] | (_tablet._buf[5] << 8);
		x = max(left, x);
		y = max(top, y);
		x = (min(right, x) - left) / width * 32767.f;
		y = (min(bottom, y) - top) / height * 32767.f;
		_vmulti._buf[4] = x;
		_vmulti._buf[5] = x >> 8;
		_vmulti._buf[6] = y;
		_vmulti._buf[7] = y >> 8;

		_vmulti.Write();
	}
}