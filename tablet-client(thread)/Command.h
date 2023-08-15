#pragma once
#include <map>
#include <functional>
#include <algorithm>

#include "Tablet.h"
#include "Mapper.h"
#include "Parse.h"

class Command final {
public:
	explicit Command(void);
	~Command(void);
public:
	void Execute(std::string line);
private:
	void Create(void);
	void Add(std::string key, std::function<void(Parse&)> func);
public:
	Tablet* _tablet = nullptr;
	Mapper* _mapper = nullptr;
private:
	std::map<std::string, std::function<void(Parse&)>> _func;
};

