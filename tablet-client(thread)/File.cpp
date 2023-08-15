#include "File.h"

File::File(void) {
}

File::~File(void) {
}

std::string File::operator()(const std::string& path) {
	std::ifstream ifs;

	ifs.open(path);
	if (!ifs.is_open())
		ifs.close();

	std::string line{ std::istreambuf_iterator<char>{ifs}, {} };
	ifs.close();

	return line;
}