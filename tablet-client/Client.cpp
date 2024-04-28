#include "Client.h"

Client::Client(Setting const& setting)
	: _tablet(Device::Config{ 0x056A, 0x030E, 0xFF0D, 0x0001, FILE_READ_DATA }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) {
}

void Client::Run(void) noexcept {
	/*register*/ int const left = _setting._area._left, right = _setting._area._right, top = _setting._area._top, bottom = _setting._area._bottom;
	/*register*/ float const width = _setting._area._width, height = _setting._area._height;
	for (;;) {
		if (!_tablet.Read())
			continue;

		_vmulti._buf[3] = _tablet._buf[1] & 0x7;
		/*register*/ int x = (_tablet._buf[2] | (_tablet._buf[3] << 8)) - left;
		/*register*/ int y = (_tablet._buf[4] | (_tablet._buf[5] << 8)) - top;

		if (0 > x)
			x = 0;
		else if (right < x)
			x = right;
		x = x / width * 32767;

		if (0 > y)
			y = 0;
		else if (bottom < y)
			y = bottom;
		y = y / height * 32767;

		_vmulti._buf[4] = x;
		_vmulti._buf[5] = x >> 8;
		_vmulti._buf[6] = y;
		_vmulti._buf[7] = y >> 8;

		_vmulti.Write();
	}
}