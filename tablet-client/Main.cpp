#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "avrt.lib")

#include "Client.h"

#include <avrt.h>

void Init(void);

int main(int argc, char** argv) {
	Init();
	Client client;
	client.Run();
}

void Init(void) {
	HANDLE handle = nullptr;
	DWORD mode = 0;
	handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &mode);
	mode = (mode & ~ENABLE_WINDOW_INPUT);
	mode = (mode & ~ENABLE_MOUSE_INPUT);
	mode = (mode & ~ENABLE_QUICK_EDIT_MODE);
	mode = (mode & ~ENABLE_EXTENDED_FLAGS);
	mode = (mode & ~ENABLE_AUTO_POSITION);
	mode = (mode & ~ENABLE_VIRTUAL_TERMINAL_INPUT);
	SetConsoleMode(handle, mode);

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(handle, &mode);
	mode = (mode & ~ENABLE_WRAP_AT_EOL_OUTPUT);
	mode = (mode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(handle, mode);

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	DWORD taskIdx = 0;
	HANDLE avrthandle = AvSetMmThreadCharacteristics(L"Games", &taskIdx);
	AvSetMmThreadPriority(avrthandle, AVRT_PRIORITY_CRITICAL);
}