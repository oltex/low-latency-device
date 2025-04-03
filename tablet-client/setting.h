#pragma once

struct setting final {
	struct area final {
		inline explicit area(int const x, int const y, int const width, int const height) noexcept
			: _left(x - width / 2), _right(_left + width), _top(y - height / 2), _bottom(_top + height),
			_width(width), _height(height) {
		}
		unsigned short const _left, _right, _top, _bottom;
		unsigned short const _width, _height;
	};
	area const _area;
};