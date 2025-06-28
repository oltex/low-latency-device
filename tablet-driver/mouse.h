#pragma once
#include <WinUser.h>
#include <SetupAPI.h>
#pragma comment(lib, "user32.lib")

class mouse final {
public:
	inline explicit mouse(void) noexcept {
		_input.type = INPUT_MOUSE;
		_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK;
	}

	inline void write(void) noexcept {
		SendInput(1, &_input, sizeof(INPUT));
	}

	INPUT _input{};
};

//using NtSendInput = BOOLEAN(WINAPI*)(UINT cInputs, LPINPUT pInputs, int cbSize);
//_nt_send_input = reinterpret_cast<NtSendInput>(GetProcAddress(GetModuleHandleW(L"win32u.dll"), "NtUserSendInput"));
//_nt_send_input(1, &_input, sizeof(INPUT));
//NtSendInput _nt_send_input;