#include "application.h"

int main(int const argc, char const* const* const argv) noexcept {
	application app{ area{atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])} };
	app.run();
	return 0;
}