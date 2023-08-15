#include "Command.h"
#include "File.h"

Command::Command(void) {
	Create();
}

Command::~Command(void) {
}

void Command::Execute(std::string line) {
	Parse parse;
	while (!line.empty()) {
		line = parse(line);
		if (0 == _func.count(parse._key))
			continue;
		_func[parse._key](parse);
	}
}

void Command::Create(void) {
	Add("Include", [&](Parse& parse) {
		File file;
	std::string line = file(parse.String(0));
	Execute(line);
	return;
		});

	Add("Tablet", [&](Parse& parse) {
		_tablet = new Tablet{
			{ parse.UShort(0), parse.UShort(1), parse.UShort(2), parse.UShort(3) },
			{ parse.UChar(5), parse.UChar(7), parse.UChar(9)} };
	return;
		});

	Add("TabletArea", [&](Parse& parse) {
		_mapper->_setting._tablet = { parse.Int(0), parse.Int(1), parse.Int(2), parse.Int(3) };
	return;
		});
}

void Command::Add(std::string key, std::function<void(Parse&)> func) {
	//std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	_func.emplace(key, func);
}