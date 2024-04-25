#pragma once

struct Setting final {
	struct Area final {
		Area(const int x, const int y, const int width, const int height)
			: _width(static_cast<float>(width)), _height(static_cast<float>(height)),
			_left(x - width / 2), _right(x + width / 2),
			_top(y - height / 2), _bottom(y + height / 2) {
		}
		float _width, _height;
		int _left, _right, _top, _bottom;
	};
	Area const _area;
};