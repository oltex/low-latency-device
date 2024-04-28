#pragma once

struct Setting final {
	struct Area final {
		Area(int const x, int const y, int const width, int const height)
			: _left(x - width / 2), _right(width),
			_top(y - height / 2), _bottom(height) {
		}
		int _left, _right, _top, _bottom;
	};
	Area const _area;
};