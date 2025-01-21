#pragma once
#include <WinUser.h>
#include <SetupAPI.h>
#pragma comment(lib, "user32.lib")

class mouse final {
public:
	inline explicit mouse(void) noexcept {
		_input.type = INPUT_MOUSE;
		_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	}
	inline ~mouse(void) noexcept = default;
public:
	inline void write(void) noexcept {
		SendInput(1, &_input, sizeof(INPUT));
	}
public:
	INPUT _input{};
	unsigned char _button = 0;
};