module;
#include <Windows.h>
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
	inline application(void) noexcept {
		::SetPriorityClass(::GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	};

	inline void run(void) noexcept {
		for (;;) {
			while (!_mouse.connect()) {
				_notify.wait();
			}
			fputs("RUNNING: reading mouse reports\n", stdout);
			while (auto const report = _mouse.read()) {
				_input.write(report->_button, report->_x, report->_y, report->_wheel);
			}
		}
	};
};
