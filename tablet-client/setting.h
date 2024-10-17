#pragma once

struct setting final {
	struct area final {
		inline explicit area(int const x, int const y, int const width, int const height) noexcept
			: _left(x - width / 2), _right(x + width / 2), _top(y - height / 2), _bottom(y + height / 2),
			_width(width), _height(height) {
		}
		unsigned short const _left, _right, _top, _bottom;
		unsigned short const _width, _height;
	};
	area const _area;
};