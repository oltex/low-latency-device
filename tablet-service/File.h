#pragma once
#include <string>
#include <fstream>

class File final {
public:
	explicit File(void);
	~File(void);
public:
	std::string operator()(const std::string& path);
};