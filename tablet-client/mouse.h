#pragma once
#include <WinUser.h>
#include <SetupAPI.h>
#pragma comment(lib, "user32.lib")

class mouse final {
public:
	inline void write(void) noexcept {
		mouse_event(_flag, _x, _y, 0, 0);
	}
	unsigned long _flag;
	unsigned long _x;
	unsigned long _y;
};