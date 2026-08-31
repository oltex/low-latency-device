module;
#include <Windows.h>
export module application;
import audio;
import tablet;
import mouse;
import notify;
import area;
import <stdio.h>;

export class application final {
	audio const _audio;
	notify const _notify;
	tablet _tablet;
	mouse  _mouse;
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
			::fputs("RUNNING: reading pen reports\n", stdout);
			while (auto const report = _tablet.read()) {
				//_mouse.write(report->_mask & 0x1,
				//	report->_x * 65535 / _area._width,
				//	report->_y * 65535 / _area._height);
				_mouse.write(report->_mask & 0x1,
					static_cast<float>(report->_x) / _area._width * 65535.f,
					static_cast<float>(report->_y) / _area._height * 65535.f);
			}
		}
	};
};

//#pragma comment(lib, "avrt.lib")
//#include <avrt.h>
//#include <shellscalingapi.h>
//#pragma comment(lib, "Shcore.lib")
//#pragma comment(lib, "winmm.lib")

//FreeConsole();
//
//PROCESS_DPI_AWARENESS
//SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
//SetProcessPriorityBoost(GetCurrentProcess(), false);
//SetThreadPriorityBoost(GetCurrentThread(), false);
//
//PROCESS_POWER_THROTTLING_STATE process_info{
//	.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION,
//	.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED,
//	.StateMask = 0 };
//SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, reinterpret_cast<void*>(&process_info), sizeof(PROCESS_POWER_THROTTLING_STATE));
//THREAD_POWER_THROTTLING_STATE thread_info{
//	.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION,
//	.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED,
//	.StateMask = 0 };
//SetThreadInformation(GetCurrentThread(), ThreadPowerThrottling, reinterpret_cast<void*>(&thread_info), sizeof(THREAD_POWER_THROTTLING_STATE));
//
//unsigned long index = 0;
//AvSetMmThreadPriority(AvSetMmThreadCharacteristicsW(L"Games", &index), AVRT_PRIORITY_CRITICAL);
//timeBeginPeriod(1);


				//	(report->_x/* - _area._left*/) * 65535 / _area._width,
				//	(report->_y/* - _area._top*/) * 65535 / _area._height);