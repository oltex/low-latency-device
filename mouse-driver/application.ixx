module;
#include <Windows.h>
#pragma comment(lib, "user32.lib")
export module application;
import mouse;
import input;
import notify;
import <stdio.h>;

export class application final {
	notify const _notify;
	mouse _mouse;
	input _input;
public:
	inline application(bool const show) noexcept {
		::SetConsoleMode(::GetStdHandle(STD_INPUT_HANDLE), ENABLE_INSERT_MODE);
		::SetConsoleMode(::GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);
		CONSOLE_CURSOR_INFO cursor_info{
			.dwSize = 1,
			.bVisible = FALSE};
		::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

		::SetPriorityClass(::GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

		if (!show)
			::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	};

	inline void run(void) noexcept {
		for (;;) {
			while (!_mouse.connect()) {
				_notify.wait();
			}
			::fputs("[RUNNING] reading mouse reports\n", stdout);
			while (auto const report = _mouse.read()) {
				_input.write(report->_button, report->_x, report->_y, report->_wheel);
			}
		}
	};
};
