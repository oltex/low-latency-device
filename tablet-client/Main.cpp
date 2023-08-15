#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "avrt.lib")

#include "Client.h"

#include <avrt.h>
#include <stdlib.h>

void Init(void);

int main(int argc, char** argv) {
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	DWORD taskIdx = 0;
	if (0 == AvSetMmThreadCharacteristics(L"Games", &taskIdx))
		return 0;
	Init();

	Client client({ { atoi(argv[1]),atoi(argv[2]), atoi(argv[3]), atoi(argv[4])} });
	client.Run();
}

void Init(void) {
	HANDLE handle;
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
}