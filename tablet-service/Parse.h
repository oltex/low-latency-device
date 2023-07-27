#pragma once
#include <string>
#include <vector>

class Parse final {
public:
	explicit Parse(void);
	~Parse(void);
public:
	std::string operator()(const std::string& line);
public:
	bool Bool(const int& idx);
	unsigned char UChar(const int& idx);
	unsigned short UShort(const int& idx);
	int Int(const int& idx);
	float Float(const int& idx);
	std::string String(const int& idx);
private:
	std::string Hax(std::string value);
public:
	std::string _key;
	std::vector<std::string> _value;
};