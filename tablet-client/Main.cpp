#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "avrt.lib")

#include "Client.h"

#include <stdio.h>
#include <avrt.h>

void Init(void);

int main(int argc, char** argv) {
	fputs("oltex-tablet-client", stdout);
	Init();
	//Client client{ Setting{ Setting::Area{atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])} } };
	Client client{ Setting{ Setting::Area{7600, 4750, 7200, 4050} } };
	client.Run();
	return 0;
}

void Init(void) {
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(handle, &mode);
	mode &= ~(ENABLE_PROCESSED_INPUT | /*ENABLE_LINE_INPUT |*/
		/*ENABLE_ECHO_INPUT |*/ ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT | /*ENABLE_INSERT_MODE |*/
		ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_AUTO_POSITION | ENABLE_VIRTUAL_TERMINAL_INPUT);
	SetConsoleMode(handle, mode);
	CloseHandle(handle);

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(handle, &mode);
	mode &= ~(/*ENABLE_PROCESSED_OUTPUT |*/  ENABLE_WRAP_AT_EOL_OUTPUT |
		ENABLE_VIRTUAL_TERMINAL_PROCESSING /*| ENABLE_LVB_GRID_WORLDWIDE*/);
	/*mode |= DISABLE_NEWLINE_AUTO_RETURN;*/
	SetConsoleMode(handle, mode);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &info);

	//const char* text = "oltex-tablet-client";
	//CHAR_INFO buf[19];
	//memset(buf, 0, sizeof(CHAR_INFO) * 19);
	//for (int i = 0; i < 19; i++) {
	//	buf[i].Char.UnicodeChar = text[i];
	//	buf[i].Attributes = 0x7;
	//}
	//SMALL_RECT rect = { 0, 0, 19, 1 };
	//WriteConsoleOutput(handle, buf, COORD{ 19, 1 }, COORD{ 0,0 }, &rect);
	CloseHandle(handle);
	FreeConsole();

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	DWORD taskIdx = 0;
	HANDLE avrthandle = AvSetMmThreadCharacteristics(L"Games", &taskIdx);
	AvSetMmThreadPriority(avrthandle, AVRT_PRIORITY_CRITICAL);
}