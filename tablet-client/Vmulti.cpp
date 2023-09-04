#include "Vmulti.h"

Vmulti::Vmulti(void)
	: _device({ 0x00EE, 0xBACC, 0xFF00, 0x0001, FILE_WRITE_DATA }) {
	memset(_buf, 0, 7);
}

Vmulti::~Vmulti(void) {
}