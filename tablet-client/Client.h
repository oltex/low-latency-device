#pragma once
#include "Tablet.h"
#include "Vmulti.h"
#include "Setting.h"

class Client final {
public:
	explicit Client(Setting const& setting) noexcept;
	~Client(void) noexcept;
public:
	void Run(void) noexcept;
private:
	Tablet _tablet;
	Setting const _setting;
	Vmulti _vmulti;
};