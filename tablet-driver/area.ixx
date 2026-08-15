export module area;
import std;
import <stdio.h>;

export class area final {
public:
	unsigned short _width, _height;
public:
	inline area(void) noexcept {
		fputs("Configuring Area...\n", stdout);
		fputs(" Width: ", stdout);
		scanf_s("%hu", &_width);
		fputs(" Height: ", stdout);
		scanf_s("%hu", &_height);
		fputs("\n", stdout);

	}
};

//: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
