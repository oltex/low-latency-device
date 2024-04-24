#include "Client.h"

Client::Client(Setting&& setting)
	: _tablet({ 0x056A, 0x030E, 0xFF0D, 0x0001, FILE_READ_DATA }, { 0x02, 10, 0x40 }),
	_setting(std::move(setting)) {
}

Client::~Client(void) {
}

#include <iostream>
void Client::Run(void) noexcept {
	register const int left = _setting._area._left, right = _setting._area._right, top = _setting._area._top, bottom = _setting._area._bottom;
	register const float width = _setting._area._width, height = _setting._area._height;

	//__int64 avg = 0, cnt = 0;
	for (;;) {
		//__int64 tsc = __rdtsc();
		if (!_tablet.Read())
			continue;

		register int x = max(left, _tablet._report.x);
		register int y = max(top, _tablet._report.y);
		_vmulti._report.x = static_cast<USHORT>((min(right, x) - left) / width * 32767.f);
		_vmulti._report.y = static_cast<USHORT>((min(bottom, y) - top) / height * 32767.f);
		_vmulti._report.button = _tablet._report.button & 0x7;

		_vmulti.Write();

		//avg += (__rdtsc() - tsc);
		//cnt++;
		//std::cout << avg / cnt << std::endl;
	}
}