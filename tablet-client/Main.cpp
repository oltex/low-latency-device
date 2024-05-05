#include "Client.h"

//#include <stdio.h>
#pragma comment(lib, "avrt.lib")
#include <avrt.h>

void Init(void);

void main(int const argc, char const* const* const argv) noexcept {
	Init();
	Client client{ Setting{ Setting::Area{atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])} } };
	client.Run();
}

void Init(void) {
	//fputs("oltex-tablet-client", stdout);

	//HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	//DWORD mode;
	//GetConsoleMode(handle, &mode);
	//mode &= ~(ENABLE_PROCESSED_INPUT | /*ENABLE_LINE_INPUT |*/
	//	/*ENABLE_ECHO_INPUT |*/ ENABLE_WINDOW_INPUT |
	//	ENABLE_MOUSE_INPUT | /*ENABLE_INSERT_MODE |*/
	//	ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS |
	//	ENABLE_AUTO_POSITION | ENABLE_VIRTUAL_TERMINAL_INPUT);
	//SetConsoleMode(handle, mode);
	////CloseHandle(handle);

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
	////CloseHandle(handle);

	//FreeConsole();

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	DWORD taskIdx = 0;
	HANDLE avrthandle = AvSetMmThreadCharacteristics("Games", &taskIdx);
	AvSetMmThreadPriority(avrthandle, AVRT_PRIORITY_CRITICAL);
}