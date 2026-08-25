module;
#include <Windows.h>
export module application;
import audio;
import tablet;
import input;
import notify;
import area;
import <stdio.h>;

export class application final {
	audio const _audio;
	notify const _notify;
	tablet _tablet;
	input  _input;
	area const _area;
public:
	inline application(int const width, int const height) noexcept
		: _area(width, height) {
		::SetConsoleMode(::GetStdHandle(STD_INPUT_HANDLE), ENABLE_INSERT_MODE);
		::SetConsoleMode(::GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);
		CONSOLE_CURSOR_INFO cursor_info{
			.dwSize = 1,
			.bVisible = FALSE};
		::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

		::SetPriorityClass(::GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	};

	inline void run(void) noexcept {
		_audio.initialize();
		for (;;) {
			while (!_tablet.connect()) {
				_notify.wait();
			}
			fputs("RUNNING: reading pen reports\n", stdout);
			while (auto const report = _tablet.read()) {
				//_input.write(report->_mask & 0x1,
				//	report->_x * 65535 / _area._width,
				//	report->_y * 65535 / _area._height);
				_input.write(report->_mask & 0x1,
					static_cast<float>(report->_x) / _area._width * 65535.f,
					static_cast<float>(report->_y) / _area._height * 65535.f);
			}
		}
	};
};