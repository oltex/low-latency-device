#include "Client.h"

Client::Client(Setting const& setting) noexcept
	: _tablet(Device::Config{ FILE_READ_DATA, 0x056A, 0x030E, 0xFF0D, 0x0001 }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) noexcept {
}

void Client::Run(void) noexcept {
	register unsigned short x, y;
	register unsigned short const left = _setting._area._left, right = _setting._area._right, top = _setting._area._top, bottom = _setting._area._bottom;
	register unsigned short const width = _setting._area._right - _setting._area._left, height = _setting._area._bottom - _setting._area._top;
	for (;;) {
		if (!_tablet.Read())
			continue;

		_vmulti._buf[3] = _tablet._buf[1] & 0x7;
		x = _tablet._buf[2] | (_tablet._buf[3] << 8);
		y = _tablet._buf[4] | (_tablet._buf[5] << 8);

		if (right < x)
			x = right;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 4) = (x - left) * 32767 / width;
		if (bottom < y)
			y = bottom;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 6) = (y - top) * 32767 / height;

		_vmulti.Write();
	}
}