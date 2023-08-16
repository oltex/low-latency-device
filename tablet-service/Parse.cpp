#include "Parse.h"

Parse::Parse(void) {
}

Parse::~Parse(void) {
}

std::string Parse::operator()(const std::string& line) {
	_key.clear();
	_value.clear();

	std::vector<std::string> sentence;
	std::string word;

	char comment = '#';
	char split[] = { ' ', '\n', };
	char end[] = { ';', 0 };
	char enclose = '"';

	bool is_enclose = false;

	int idx = 0; //temp

	for (auto letter : line) {
		idx++; // temp
		bool is_split = false;
		for (int i = 0; i < (int)sizeof(split); i++)
			if (split[i] == letter) {
				is_split = true;
				break;
			}

		bool is_end = false;
		for (int i = 0; i < (int)sizeof(end); i++)
			if (end[i] == letter) {
				is_end = true;
				break;
			}

		if (enclose == letter) {
			is_enclose = !is_enclose;
			continue;
		}

		if (!is_enclose && (is_split || is_end)) {
			if (!word.empty()) {
				sentence.push_back(word);
				word.clear();
			}
			if (is_end)
				break;
		}
		else {
			word.push_back(letter);
		}
	}
	if (!word.empty())
		sentence.push_back(word);

	_key = sentence.front();
	_value.insert(_value.begin(), sentence.begin() + 1, sentence.end());

	return line.substr(idx, line.size());
}

bool Parse::Bool(const int& idx) {
	std::string value = _value[idx];
	if ("true" == value)
		return true;
	return false;
}

unsigned char Parse::UChar(const int& idx) {
	return static_cast<unsigned char>(std::stoi(Hax(_value[idx])));
}

unsigned short Parse::UShort(const int& idx) {
	return static_cast<unsigned short>(std::stoi(Hax(_value[idx])));
}

int Parse::Int(const int& idx) {
	return std::stoi(Hax(_value[idx]));
}

float Parse::Float(const int& idx) {
	return std::stof(_value[idx]);
}

std::string Parse::String(const int& idx) {
	return _value[idx];
}

std::string Parse::Hax(std::string value) {
	if (value.size() >= 3 && value[0] == '0' && value[1] == 'x') {
		try {
			std::string tmp = value.substr(2, value.size() - 2);
			return std::to_string(stol(tmp, 0, 16));
		}
		catch (std::exception) {
		}
	}
	return value;
}
