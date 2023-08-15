#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

#include <iostream>

#include "Tablet.h"
#include "Vmulti.h"
#include "Mapper.h"
#include "Service.h"

#include "Command.h"
#include "File.h"

void Init(void);
void ReadFile(char** argv);

Tablet* tablet = nullptr;
Vmulti* vmulti = nullptr;
Mapper* mapper = nullptr;
Service* service = nullptr;

Command* command = nullptr;


int main(int argc, char** argv) {
	Init();

	tablet = new Tablet{ {0x056A, 0x030E, 0xFF0D, 0x0001}, { 0x02, 10, 0x40 } };
	vmulti = new Vmulti;
	mapper = new Mapper;
	service = new Service;

	mapper->_setting._tablet;
	mapper->_setting._screen = Mapper::Setting::Area{ 0, 0, 1920, 1080 };
	mapper->_tablet = tablet;
	mapper->_vmulti = vmulti;

	service->_tablet = tablet;
	service->_vmulti = vmulti;
	service->_mapper = mapper;

	command = new Command;
	command->_tablet = tablet;
	command->_mapper = mapper;

	ReadFile(argv);

	command->Execute("TabletArea 7200 4050 7600 4750");
	service->Run();

	std::string line;
	while (true) {
		std::getline(std::cin, line);
	}

	delete(service);
	delete(tablet);
	delete(mapper);
	delete(vmulti);
}

void Init(void) {
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);

	HANDLE handle;
	DWORD mode = 0;
	handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &mode);
	//mode = (mode & ~ENABLE_PROCESSED_INPUT);
	//mode = (mode & ~ENABLE_LINE_INPUT);
	//mode = (mode & ~ENABLE_ECHO_INPUT);
	//mode = (mode & ~ENABLE_INSERT_MODE);
	mode = (mode & ~ENABLE_WINDOW_INPUT);
	mode = (mode & ~ENABLE_MOUSE_INPUT);
	mode = (mode & ~ENABLE_QUICK_EDIT_MODE);
	mode = (mode & ~ENABLE_EXTENDED_FLAGS);
	mode = (mode & ~ENABLE_AUTO_POSITION);
	mode = (mode & ~ENABLE_VIRTUAL_TERMINAL_INPUT);
	SetConsoleMode(handle, mode);

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(handle, &mode);
	//consoleMode = (consoleMode & ~ENABLE_PROCESSED_OUTPUT);
	mode = (mode & ~ENABLE_WRAP_AT_EOL_OUTPUT);
	mode = (mode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(handle, mode);
}

void ReadFile(char** argv) {
	std::string path;
	path = argv[1];

	File file;
	std::string line = file(path);
	command->Execute(line);
}