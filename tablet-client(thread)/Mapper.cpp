#include "Mapper.h"

Mapper::Mapper(void) {
	memset(&_setting, 0, sizeof(Setting));
}

Mapper::~Mapper(void) {
}

void Mapper::Convert(void) noexcept {
	register float x, y;
	register UCHAR button;

	x = (min(_setting._tablet._right, max(_setting._tablet._left, _tablet->_report.x)) - _setting._tablet._left) / float(_setting._tablet._width);
	y = (min(_setting._tablet._bottom, max(_setting._tablet._top, _tablet->_report.y)) - _setting._tablet._top) / float(_setting._tablet._height);

	button = _tablet->_report.button & 0x0F;
	for (int buttonIdx = 0; buttonIdx < 3; ++buttonIdx) {
		if (0 == (1 << buttonIdx & button))
			continue;
	}

	_vmulti->_report.button = _tablet->_report.button & 0x0F;
	_vmulti->_report.x = USHORT(x * 32767.f);
	_vmulti->_report.y = USHORT(y * 32767.f);
}
