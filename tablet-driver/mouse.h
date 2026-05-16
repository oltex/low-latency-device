#pragma once
#pragma comment(lib, "user32.lib")
#include <WinUser.h>

class mouse final {
public:
	INPUT _input{};

	inline explicit mouse(void) noexcept {
		_input.type = INPUT_MOUSE;
	}

	inline void write(void) noexcept {
		//::SetCursorPos(_input.mi.dx, _input.mi.dy);
		::SendInput(1, &_input, sizeof(INPUT));
	}
};


//using NtSendInput = BOOLEAN(WINAPI*)(UINT cInputs, LPINPUT pInputs, int cbSize);
//_nt_send_input = reinterpret_cast<NtSendInput>(GetProcAddress(GetModuleHandleW(L"win32u.dll"), "NtUserSendInput"));
//_nt_send_input(1, &_input, sizeof(INPUT));
//NtSendInput _nt_send_input;