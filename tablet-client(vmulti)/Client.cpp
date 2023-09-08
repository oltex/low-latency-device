#include "Client.h"

Client::Client(const Setting& setting)
	: _tablet({ 0x056A, 0x030E, 0xFF0D, 0x0001, FILE_READ_DATA }, { 0x02, 10, 0x40 }),
	_setting(setting) {
}

Client::~Client(void) {
}

void Client::Run(void) noexcept {
	register int x, y;
	while (true) {
		if (!_tablet.Read())
			continue;

		x = max(_setting._area._left, _tablet._report.x);
		y = max(_setting._area._top, _tablet._report.y);
		_vmulti._report.x = USHORT((min(_setting._area._right, x) - _setting._area._left) / _setting._area._width * 32767.f);
		_vmulti._report.y = USHORT((min(_setting._area._bottom, y) - _setting._area._top) / _setting._area._height * 32767.f);
		_vmulti._report.button = _tablet._report.button & 0x0F;

		_vmulti.Write();
	}
}