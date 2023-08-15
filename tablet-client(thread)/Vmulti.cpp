#include "Vmulti.h"

Vmulti::Vmulti(void) {
	_device = new Device({ 0x00FF, 0xBACC, 0xFF00, 0x0001 });
	memset(_buf, 0, 65);
}

Vmulti::~Vmulti(void) {
	delete _device;
}

int Vmulti::Write(void) noexcept {
	memcpy(_buf, &_report, sizeof(Report));
	return _device->Write(_buf, 65);
}