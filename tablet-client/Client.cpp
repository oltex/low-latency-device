#include "Client.h"

Client::Client(Setting const& setting) noexcept
	: _tablet(Device::Config{ FILE_READ_DATA, 0x056A, 0x030E, 0xFF0D, 0x0001 }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) noexcept {
}

void Client::Run(void) noexcept {
	for (;;) {
		if (!_tablet.Read())
			continue;

		register unsigned short x = _tablet._buf[2] | (_tablet._buf[3] << 8);
		register unsigned short y = _tablet._buf[4] | (_tablet._buf[5] << 8);
		_vmulti._buf[3] = _tablet._buf[1] & 0x7;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 4) = (min(x, _setting._area._right) - _setting._area._left) * 32767 / _setting._area._width;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 6) = (min(y, _setting._area._bottom) - _setting._area._top) * 32767 / _setting._area._height;

		_vmulti.Write();
	}
}