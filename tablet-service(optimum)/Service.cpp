#include "Service.h"

Service::Service(void)
	: _tablet({ 0x056A, 0x030E, 0xFF0D, 0x0001, GENERIC_READ }, { 0x02, 10, 0x40 }),
	_setting({ { 7600, 4750, 7200, 4050 } }) {
}

Service::~Service(void) {
}

void Service::Run(void) noexcept {
	register int x, y;
	register const int left = _setting._area._left, right = _setting._area._right, top = _setting._area._top, bottom = _setting._area._bottom;
	register const float width = _setting._area._width, height = _setting._area._height;
	while (true) {
		if (!_tablet.Read())
			continue;

		x = max(left, _tablet._report.x);
		y = max(top, _tablet._report.y);
		_vmulti._report.x = USHORT((min(right, x) - left) / width * 32767.f);
		_vmulti._report.y = USHORT((min(bottom, y) - top) / height * 32767.f);
		_vmulti._report.button = _tablet._report.button & 0x0F;

		_vmulti.Write();
	}
}