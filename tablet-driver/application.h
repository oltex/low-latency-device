#pragma once
#include "tablet.h"
#include "mouse.h"
//#pragma comment(lib, "avrt.lib")
//#include <avrt.h>
#include <stdio.h>
//#include <shellscalingapi.h>
//#pragma comment(lib, "Shcore.lib")
//#pragma comment(lib, "winmm.lib")

struct area final {
	inline explicit area(int const x = 7360, int const y = 4600, int const width = 8544, int const height = 4806) noexcept
		: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
	}
	unsigned short const _left, _top;
	unsigned short const _width, _height;
};

class application final {
public:
	inline explicit application(void) noexcept {
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);
		CONSOLE_CURSOR_INFO cursor_info{
			.dwSize = 1,
			.bVisible = FALSE };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		fputs("Tablet Driver\n", stdout);
		fputs("by oltex\n", stdout);
		//FreeConsole();

		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		//PROCESS_DPI_AWARENESS
		//SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		//SetProcessPriorityBoost(GetCurrentProcess(), false);
		//SetThreadPriorityBoost(GetCurrentThread(), false);

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

		//unsigned long index = 0;
		//AvSetMmThreadPriority(AvSetMmThreadCharacteristicsW(L"Games", &index), AVRT_PRIORITY_CRITICAL);
		//timeBeginPeriod(1);
	};
	inline void run(void) noexcept {
		unsigned char _button = 0;
		for (;;) {
			_tablet.read();

			unsigned char button = _tablet._buffer[1] & 0x1; //0x7
			if (_button ^ button) {
				_mouse._input.mi.dwFlags = (button << 1) + (_button << 2);
				SendInput(1, &_mouse._input, sizeof(INPUT));
				_button = button;
			}
			_mouse._input.mi.dx = (*reinterpret_cast<unsigned short*>(_tablet._buffer + 2) - _area._left) * 1920 / _area._width;
			_mouse._input.mi.dy = (*reinterpret_cast<unsigned short*>(_tablet._buffer + 4) - _area._top) * 1080 / _area._height;

			_mouse.write();
		}
	};
private:
	tablet _tablet;
	area const _area;
	mouse _mouse;
};