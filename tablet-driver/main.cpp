#include <Windows.h>
import application;

int main(int, char* argv[]) noexcept {
	application app(atoi(argv[1]), atoi(argv[2]));
	app.run();
	return 0;
}