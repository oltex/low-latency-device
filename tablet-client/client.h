#pragma once
#include "tablet.h"
#include "mouse.h"
#include "setting.h"
class client final {
public:
	inline explicit client(setting const& setting) noexcept
		: _setting(setting) {
	};
	inline ~client(void) noexcept = default;
public:
	inline void run(void) noexcept {
		for (;;) {
			if (!_tablet.read())
				continue;

			register unsigned char button = _tablet._buffer[1] & 0x7;
			register unsigned short x = *reinterpret_cast<unsigned short*>(_tablet._buffer + 2);
			register unsigned short y = *reinterpret_cast<unsigned short*>(_tablet._buffer + 4);

			_mouse._input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
			if (!(1 & _mouse._button) && 1 & button)
				_mouse._input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
			if (1 & _mouse._button && !(1 & button))
				_mouse._input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
			_mouse._button = button;
			_mouse._input.mi.dx = (min(x, _setting._area._right) - _setting._area._left) * 65535 / _setting._area._width;
			_mouse._input.mi.dy = (min(y, _setting._area._bottom) - _setting._area._top) * 65535 / _setting._area._height;

			_mouse.write();
		}
	};

private:
	tablet _tablet;
	setting const _setting;
	mouse _mouse;
};

//#pragma once
//#include "tablet.h"
//#include "vmulti.h"
//#include "setting.h"
//class client final {
//public:
//	inline explicit client(setting const& setting) noexcept
//		: _setting(setting) {
//	};
//	inline ~client(void) noexcept = default;
//public:
//	inline void run(void) noexcept {
//		for (;;) {
//			if (!_tablet.read())
//				continue;
//
//			register unsigned short x = *reinterpret_cast<unsigned short*>(_tablet._buffer + 2);
//			register unsigned short y = *reinterpret_cast<unsigned short*>(_tablet._buffer + 4);
//			_vmulti._buffer[3] = _tablet._buffer[1] & 0x7;
//			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 4) = (min(x, _setting._area._right) - _setting._area._left) * 32767 / _setting._area._width;
//			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 6) = (min(y, _setting._area._bottom) - _setting._area._top) * 32767 / _setting._area._height;
//
//			_vmulti.write();
//		}
//	};
//private:
//	tablet _tablet;
//	setting const _setting;
//	vmulti _vmulti;
//};