module;
#pragma comment(lib, "cfgmgr32.lib")
#include <windows.h>
#include <cfgmgr32.h>
export module notify;
import <stdio.h>;

HANDLE _event = nullptr;
DWORD _stdcall callback(HCMNOTIFICATION hNotify, PVOID Context, CM_NOTIFY_ACTION Action, PCM_NOTIFY_EVENT_DATA EventData, DWORD EventDataSize) {
	if (Action == CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL) {
		::SetEvent(_event);
	}
	return ERROR_SUCCESS;
}

export class notify {
	HCMNOTIFICATION _notify;
public:
	inline notify(void) noexcept {
		_event = ::CreateEventW(nullptr, FALSE, FALSE, nullptr);
		CM_NOTIFY_FILTER filter{};
		filter.cbSize = sizeof(filter);
		filter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE;
		filter.u.DeviceInterface.ClassGuid = {0x4D1E55B2, 0xF16F, 0x11CF, {0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}};
		::CM_Register_Notification(&filter, nullptr, callback, &_notify);
		printf("NOTIFY: watching for device events: arrival / removal\n");
	}
	inline ~notify(void) noexcept {
		::CM_Unregister_Notification(_notify);
		::CloseHandle(_event);
	}

	inline void wait(void) const noexcept {
		::WaitForSingleObject(_event, INFINITE);
	}
};