#include "Client.h"

Client::Client(Setting const& setting)
	: _tablet(Device::Config{ FILE_READ_DATA, 0x056A, 0x030E, 0xFF0D, 0x0001 }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) {
}

void Client::Run(void) noexcept {
	register int const left = _setting._area._left, width = _setting._area._right, top = _setting._area._top, height = _setting._area._bottom;
	for (;;) {
		if (!_tablet.Read())
			continue;

		_vmulti._buf[3] = _tablet._buf[1] & 0x7;
		register int x = (_tablet._buf[2] | (_tablet._buf[3] << 8)) - left;
		register int y = (_tablet._buf[4] | (_tablet._buf[5] << 8)) - top;

		if (0 > x)
			x = 0;
		else if (width < x)
			x = width;
		*reinterpret_cast<int*>(_vmulti._buf + 4) = ((x << 15) - 1) / width;

		if (0 > y)
			y = 0;
		else if (height < y)
			y = height;
		*reinterpret_cast<int*>(_vmulti._buf + 6) = ((y << 15) - 1) / height;

		_vmulti.Write();
	}
}