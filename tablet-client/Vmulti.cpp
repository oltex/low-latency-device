#include "Vmulti.h"

Vmulti::Vmulti(void)
	: _device({ 0x00FF, 0xBACC, 0xFF00, 0x0001, FILE_WRITE_DATA }) {
	memset(_buf, 0, 65);
}

Vmulti::~Vmulti(void) {
}

void Vmulti::Write(void) noexcept {
	memcpy(_buf, &_report, sizeof(Report));
	_device.Write(_buf, 65);
}