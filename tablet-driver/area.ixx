export module area;
import std;
import <stdio.h>;

export class area final {
public:
	unsigned short const _width, _height;
public:
	inline area(int const width, int const height) noexcept
		: _width(width), _height(height) {
		printf("Configuring Area...\n"\
			" Width: %hu\n"\
			" Height: %hu\n\n",
			_width, _height);
	}
};

//: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
