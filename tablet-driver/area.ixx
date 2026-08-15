export module area;
import std;
import <stdio.h>;

export class area final {
public:
	unsigned short _width, _height;
public:
	inline area(void) noexcept {
		scanf_s("%hu", &_width);
		scanf_s("%hu", &_height);
		printf("Configuring Area...\n"\
			" Width: %hu\n"\
			" Height: %hu\n\n",
			_width, _height);
	}
};

//: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
