#include "Client.h"

Client::Client(Setting const& setting) noexcept
	: _tablet(Device::Config{ FILE_READ_DATA, 0x056A, 0x030E, 0xFF0D, 0x0001 }, Tablet::Config{ 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) noexcept {
}

void Client::Run(void) noexcept {
	unsigned int const left = _setting._area._left, top = _setting._area._top;
	unsigned int const width = _setting._area._width, height = _setting._area._height;
	for (;;) {
		if (!_tablet.Read())
			continue;

		_vmulti._buf[3] = _tablet._buf[1] & 0x7;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 4) = (*reinterpret_cast<unsigned short*>(_tablet._buf + 2) - left) * 32767 / width;
		*reinterpret_cast<unsigned short*>(_vmulti._buf + 6) = (*reinterpret_cast<unsigned short*>(_tablet._buf + 4) - top) * 32767 / height;

		_vmulti.Write();
	}
}