module;
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <WinUser.h>
export module input;

export class input final {
	INPUT _input[2]{};
	unsigned char _button = 0;

public:
	inline input(void) noexcept {
		_input[0].type = INPUT_MOUSE;
		_input[1].type = INPUT_MOUSE;
	}

	inline void write(unsigned char const button, short const x, short const y, signed char const wheel) noexcept {
		UINT count = 1;
		_input[0].mi.dwFlags = MOUSEEVENTF_MOVE;
		_input[0].mi.mouseData = 0;
		if (0x1 & (button ^ _button))
			_input[0].mi.dwFlags |= (button & 0x1) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
		if (0x2 & (button ^ _button))
			_input[0].mi.dwFlags |= (button & 0x2) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
		if (0x4 & (button ^ _button))
			_input[0].mi.dwFlags |= (button & 0x4) ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
		_button = button;
		if (wheel) {
			_input[0].mi.dwFlags |= MOUSEEVENTF_WHEEL;
			_input[0].mi.mouseData = wheel * WHEEL_DELTA;
		}
		_input[0].mi.dx = x;
		_input[0].mi.dy = y;
		::SendInput(count, _input, sizeof(INPUT));
	}
};
