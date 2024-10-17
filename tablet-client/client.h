#pragma once
#include "tablet.h"
#include "vmulti.h"
#include "setting.h"

class client final {
public:
	inline explicit client(setting const& setting) noexcept
		: _tablet(device::config{ FILE_READ_DATA, 0x056A, 0x030E, 0xFF0D, 0x0001 }, tablet::config{ 0x02, 10, 0x20 }),
		_setting(setting) {
	};
	inline ~client(void) noexcept = default;
public:
	inline void run(void) noexcept {
		for (;;) {
			if (!_tablet.read())
				continue;

			register unsigned short x = *reinterpret_cast<unsigned short*>(_tablet._buffer + 2);
			register unsigned short y = *reinterpret_cast<unsigned short*>(_tablet._buffer + 4);
			_vmulti._buffer[3] = _tablet._buffer[1] & 0x7;
			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 4) = (min(x, _setting._area._right) - _setting._area._left) * 32767 / _setting._area._width;
			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 6) = (min(y, _setting._area._bottom) - _setting._area._top) * 32767 / _setting._area._height;

			_vmulti.write();
		}
	};
private:
	tablet _tablet;
	setting const _setting;
	vmulti _vmulti;
};