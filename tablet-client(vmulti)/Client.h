#pragma once
#include "Tablet.h"
#include "Vmulti.h"
#include "Setting.h"

class Client final {
public:
	explicit Client(const Setting& setting);
	~Client(void);
public:
	void Run(void) noexcept;
private:
	Tablet _tablet;
	Vmulti _vmulti;
	const Setting _setting;
};