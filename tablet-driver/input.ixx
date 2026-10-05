module;
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <WinUser.h>
export module input;
import <stdio.h>;

export class input final {
	INPUT _input{};
	unsigned char _button = 0;
	int const _width, _height;
public:
	inline input(void) noexcept
		: _width(::GetSystemMetrics(SM_CXSCREEN)), _height(::GetSystemMetrics(SM_CYSCREEN)) {
		_input.type = INPUT_MOUSE;
		::printf("[SCREEN]  primary monitor:            %dx%d\n", _width, _height);
	}

	inline void write(unsigned char const button, float const x, float const y) noexcept {
		//_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		//if (_button ^ button) {
		//	_input.mi.dwFlags |= (button << 1) + (_button << 2);
		//	_button = button;
		//}
		//_input.mi.dx = static_cast<int>(x * 65535.f);
		//_input.mi.dy = static_cast<int>(y * 65535.f);
		//::SendInput(1, &_input, sizeof(INPUT));

		::SetCursorPos(static_cast<int>(x * _width), static_cast<int>(y * _height));
		_input.mi.dwFlags = 0;
		if (_button ^ button) {
			_input.mi.dwFlags |= (button << 1) + (_button << 2);
			_button = button;
			::SendInput(1, &_input, sizeof(INPUT));
		}
	}
};


//using NtSendInput = BOOLEAN(WINAPI*)(UINT cInputs, LPINPUT pInputs, int cbSize);
//_nt_send_input = reinterpret_cast<NtSendInput>(GetProcAddress(GetModuleHandleW(L"win32u.dll"), "NtUserSendInput"));
//_nt_send_input(1, &_input, sizeof(INPUT));
//NtSendInput _nt_send_input;