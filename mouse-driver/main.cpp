#include <Windows.h>
#include <stdio.h>
import application;

int main(void) noexcept {
	::fputs("=======================================================\n"
		"  MOUSE DRIVER                         v0.1  by oltex\n"
		"=======================================================\n", stdout);
	application app;
	app.run();
	return 0;
}
