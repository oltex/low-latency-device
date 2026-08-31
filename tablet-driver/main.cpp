#include <Windows.h>
#include <stdio.h>
import application;

int main(int, char* argv[]) noexcept {
	::fputs(
		"  TABLET DRIVER                          v0.1  by oltex\n"
		"=========================================================\n", stdout);
	application app(atoi(argv[1]), atoi(argv[2]));
	app.run();
	return 0;
}