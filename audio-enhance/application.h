#pragma once
#include "audio.h"
#include <stdio.h>

class application final {
public:
	inline explicit application(void) noexcept {
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);
		CONSOLE_CURSOR_INFO cursor_info;
		cursor_info.dwSize = 1;
		cursor_info.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	};
	inline void run(void) noexcept {
		_audio.initialize();
		//_audio.get();
		Sleep(INFINITE);
	}
private:
	audio _audio;
};
