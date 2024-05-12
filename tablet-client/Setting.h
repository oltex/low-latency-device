#pragma once

struct Setting final {
	struct Area final {
		Area(int const x, int const y, int const width, int const height) noexcept
			: _left(x - width / 2), _top(y - height / 2),
			_width(width), _height(height) {
		}
		unsigned short const _left, _top;
		unsigned short const _width, _height;
	};
	Area const _area;
};