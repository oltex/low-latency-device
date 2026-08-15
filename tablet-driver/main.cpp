#include <Windows.h>
import application;

int main(int, char* argv[]) noexcept {
	application app(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	app.run();
	return 0;
}