#pragma once
#include "Tablet.h"
#include "Vmulti.h"
#include "Setting.h"

class Service final {
public:
	explicit Service(void);
	~Service(void);
public:
	void Run(void) noexcept;
private:
	Tablet _tablet;
	Vmulti _vmulti;
	const Setting _setting;
};