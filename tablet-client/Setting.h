#pragma once

struct Setting final {
	struct Area final {
		Area(int const x, int const y, int const width, int const height)
			: _left(x - width / 2), _width(width),
			_top(y - height / 2), _height(height) {
		}
		int _left, _width, _top, _height;
	};
	Area const _area;
};