#pragma once
#include "tablet.h"
#include "mouse.h"
#pragma comment(lib, "avrt.lib")
#include <avrt.h>

struct area final {
	inline explicit area(int const x, int const y, int const width, int const height) noexcept
		: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
	}
	unsigned short const _left, _top;
	unsigned short const _width, _height;
};

class client final {
public:
	inline explicit client(area const& area) noexcept
		: _area(area) {
		FreeConsole();
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

		SetProcessPriorityBoost(GetCurrentProcess(), false);
		SetThreadPriorityBoost(GetCurrentThread(), false);

		PROCESS_POWER_THROTTLING_STATE info;
		info.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
		info.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
		info.StateMask = 0;
		SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, reinterpret_cast<void*>(&info), sizeof(PROCESS_POWER_THROTTLING_STATE));
		THREAD_POWER_THROTTLING_STATE thread_info;
		thread_info.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;
		thread_info.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
		thread_info.StateMask = 0;
		SetThreadInformation(GetCurrentThread(), ThreadPowerThrottling, reinterpret_cast<void*>(&thread_info), sizeof(THREAD_POWER_THROTTLING_STATE));

		unsigned long index = 0;
		HANDLE handle = AvSetMmThreadCharacteristicsW(L"Games", &index);
		AvSetMmThreadPriority(handle, AVRT_PRIORITY_CRITICAL);
	};
	inline void run(void) noexcept {
		unsigned char _button = 0;
		for (;;) {
			_tablet.read();

			unsigned char button = _tablet._buffer[1] & 0x1; //0x7
			_mouse._input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
			if (_button ^ button) {
				_mouse._input.mi.dwFlags |= (button << 1) + (_button << 2);
				_button = button;
			}
			_mouse._input.mi.dx = (*reinterpret_cast<unsigned short*>(_tablet._buffer + 2) - _area._left) * 65535 / _area._width;
			_mouse._input.mi.dy = (*reinterpret_cast<unsigned short*>(_tablet._buffer + 4) - _area._top) * 65535 / _area._height;

			_mouse.write();
		}
	};
private:
	tablet _tablet;
	area const _area;
	mouse _mouse;
};

//#pragma once
//#include "tablet.h"
//#include "vmulti.h"
//#include "setting.h"
//class client final {
//public:
//	inline explicit client(setting const& setting) noexcept
//		: _setting(setting) {
//	};
//	inline ~client(void) noexcept = default;
//public:
//	inline void run(void) noexcept {
//		for (;;) {
//			if (!_tablet.read())
//				continue;
//
//			register unsigned short x = *reinterpret_cast<unsigned short*>(_tablet._buffer + 2);
//			register unsigned short y = *reinterpret_cast<unsigned short*>(_tablet._buffer + 4);
//			_vmulti._buffer[3] = _tablet._buffer[1] & 0x7;
//			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 4) = (min(x, _setting._area._right) - _setting._area._left) * 32767 / _setting._area._width;
//			*reinterpret_cast<unsigned short*>(_vmulti._buffer + 6) = (min(y, _setting._area._bottom) - _setting._area._top) * 32767 / _setting._area._height;
//
//			_vmulti.write();
//		}
//	};
//private:
//	tablet _tablet;
//	setting const _setting;
//	vmulti _vmulti;
//};


//fputs("oltex-tablet-client", stdout);

//HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
//unsigned long mode;
//GetConsoleMode(handle, &mode);
//mode &= ~(ENABLE_PROCESSED_INPUT | /*ENABLE_LINE_INPUT |*/
//	/*ENABLE_ECHO_INPUT |*/ ENABLE_WINDOW_INPUT |
//	ENABLE_MOUSE_INPUT | /*ENABLE_INSERT_MODE |*/
//	ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS |
//	ENABLE_AUTO_POSITION | ENABLE_VIRTUAL_TERMINAL_INPUT);
//SetConsoleMode(handle, mode);
//handle = GetStdHandle(STD_OUTPUT_HANDLE);
//GetConsoleMode(handle, &mode);
//mode &= ~(/*ENABLE_PROCESSED_OUTPUT |*/  ENABLE_WRAP_AT_EOL_OUTPUT |
//	ENABLE_VIRTUAL_TERMINAL_PROCESSING /*| ENABLE_LVB_GRID_WORLDWIDE*/);
///*mode |= DISABLE_NEWLINE_AUTO_RETURN;*/
//SetConsoleMode(handle, mode);

//CONSOLE_CURSOR_INFO info;
//info.dwSize = 1;
//info.bVisible = FALSE;
//SetConsoleCursorInfo(handle, &info);


			//if (!(_mouse._button) && button)
			//	_mouse._input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
			//if (_mouse._button && !(button))
			//	_mouse._input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;