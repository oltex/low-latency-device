module;
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <WinUser.h>
export module mouse;

export class mouse final {
	INPUT _input{};
	unsigned char _button = 0;
public:
	inline mouse(void) noexcept {
		_input.type = INPUT_MOUSE;
	}

	inline void write(unsigned char const button, int const x, int const y) noexcept {
		_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		if (_button ^ button) {
			_input.mi.dwFlags |= (button << 1) + (_button << 2);
			_button = button;
		}
		_input.mi.dx = x;
		_input.mi.dy = y;
		::SendInput(1, &_input, sizeof(INPUT));
	}
};

//::SetCursorPos(_input.mi.dx, _input.mi.dy);

//using NtSendInput = BOOLEAN(WINAPI*)(UINT cInputs, LPINPUT pInputs, int cbSize);
//_nt_send_input = reinterpret_cast<NtSendInput>(GetProcAddress(GetModuleHandleW(L"win32u.dll"), "NtUserSendInput"));
//_nt_send_input(1, &_input, sizeof(INPUT));
//NtSendInput _nt_send_input;