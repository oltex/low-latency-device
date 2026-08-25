module;
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <WinUser.h>
export module input;

export class input final {
	INPUT _input{};
	unsigned char _button = 0;

public:
	inline input(void) noexcept {
		_input.type = INPUT_MOUSE;
	}

	inline void write(unsigned char const button, short const x, short const y, signed char const wheel) noexcept {
		_input.mi.dwFlags = MOUSEEVENTF_MOVE;
		_input.mi.mouseData = 0;
		if ((button ^ _button) & 0x1)
			_input.mi.dwFlags |= (button & 0x1) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
		if ((button ^ _button) & 0x2)
			_input.mi.dwFlags |= (button & 0x2) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
		if ((button ^ _button) & 0x4)
			_input.mi.dwFlags |= (button & 0x4) ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
		_button = button;
		if (wheel) {
			_input.mi.dwFlags |= MOUSEEVENTF_WHEEL;
			_input.mi.mouseData = wheel * WHEEL_DELTA;
		}
		_input.mi.dx = x;
		_input.mi.dy = y;
		::SendInput(1, &_input, sizeof(INPUT));
	}
};
