#pragma once

struct Setting final {
	struct Area final {
		explicit Area(const int x, const int y, const int width, const int height)
			: _width(static_cast<float>(width)), _height(static_cast<float>(height)),
			_left(x - width / 2), _right(x + width / 2),
			_top(y - height / 2), _bottom(y + height / 2) {
		}
		explicit Area(Area&& rhs) noexcept = default;
		float const _width, _height;
		int const _left, _right, _top, _bottom;
	};

	explicit Setting(Setting::Area&& area)
		: _area(std::move(area)) {
	};
	explicit Setting(Setting&& rhs) noexcept 
		: _area(std::move(rhs._area)){
	};
	Area _area;
};