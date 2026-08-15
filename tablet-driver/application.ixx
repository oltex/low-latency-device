module;
#pragma comment(lib, "cfgmgr32.lib")
#include <windows.h>
#include <cfgmgr32.h>
export module application;
import tablet;
import mouse;
import <stdio.h>;

HANDLE _event = nullptr;
DWORD _stdcall callback(HCMNOTIFICATION hNotify, PVOID Context, CM_NOTIFY_ACTION Action, PCM_NOTIFY_EVENT_DATA EventData, DWORD EventDataSize) {
	if (Action == CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL) {
		::SetEvent(_event);
	}
	return ERROR_SUCCESS;
}
struct area final {
	inline explicit area(int const x, int const y, int const width, int const height) noexcept
		: _left(x - width / 2), _top(y - height / 2), _width(width), _height(height) {
	}
	unsigned short const _left, _top;
	unsigned short const _width, _height;
};

export class application final {
	tablet _tablet;
	area const _area;
	mouse _mouse;

	HCMNOTIFICATION _notify;
public:
	inline explicit application(int x, int y, int width, int height) noexcept
		: _area(x, y, width, height) {
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT);
		CONSOLE_CURSOR_INFO cursor_info{
			.dwSize = 1,
			.bVisible = FALSE};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		fputs("Tablet Driver\n", stdout);

		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

		_event = ::CreateEventW(nullptr, FALSE, FALSE, nullptr);
		CM_NOTIFY_FILTER filter{};
		filter.cbSize = sizeof(filter);
		filter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE;
		filter.u.DeviceInterface.ClassGuid = {0x4D1E55B2, 0xF16F, 0x11CF, {0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}};
		::CM_Register_Notification(&filter, nullptr, callback, &_notify);
	};
	~application(void) noexcept {
		::CM_Unregister_Notification(_notify);
		::CloseHandle(_event);
	}

	inline void run(void) noexcept {
		for (;;) {
			//fputs("Connecting to Tablet Driver...", stdout);
			while (!_tablet.connect()) {
				::WaitForSingleObject(_event, INFINITE);
			}
			while (auto const report = _tablet.read()) {
				_mouse.write(report->_mask & 0x1,
					(report->_x - _area._left) * 65535 / _area._width,
					(report->_y - _area._top) * 65535 / _area._height);
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