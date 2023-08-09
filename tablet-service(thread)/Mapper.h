#pragma once
#include "Tablet.h"
#include "Vmulti.h"

class Mapper final {
public:
	struct Setting {
		struct Area {
			int _x, _y;
			int _width, _height;
			int _left, _right, _top, _bottom;
			explicit Area(void) = default;
			Area(int x, int y, int width, int height)
				: _x(x), _y(y), _width(width), _height(height) {
				_left = x - _width / 2;
				_right = x + _width / 2;
				_top = y - height / 2;
				_bottom = y + height / 2;
			}
		};
		Area _screen, _tablet;
	};
public:
	explicit Mapper(void);
	~Mapper(void);
public:
	void Convert(void) noexcept;
public:
	Tablet* _tablet = nullptr;
	Vmulti* _vmulti = nullptr;
	Setting _setting;
};