#include <Windows.h>
#include <stdio.h>
#include <string.h>
import application;

int main(int const argc, char* argv[]) noexcept {
	::fputs("=======================================================\n"
		"  MOUSE DRIVER                         v0.1  by oltex\n"
		"=======================================================\n", stdout);
	bool const show = !(1 < argc && 0 == ::strcmp(argv[1], "hide"));
	application app(show);
	app.run();
	return 0;
}
