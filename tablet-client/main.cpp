#include "client.h"

int main(int const argc, char const* const* const argv) noexcept {
	client client{ setting{ setting::area{atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])} } };
	client.run();
	return 0;
}