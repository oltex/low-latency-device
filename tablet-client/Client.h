#pragma once
#include "Tablet.h"
#include "Vmulti.h"
#include "Setting.h"

class Client final {
public:
	explicit Client(Setting&& setting);
	~Client(void);
public:
	void Run(void) noexcept;
private:
	Tablet _tablet;
	Vmulti _vmulti;
	Setting const _setting;
};