#include "Vmulti.h"

Vmulti::Vmulti(void) noexcept
	//: _device(Device::Config{ FILE_WRITE_DATA, 0x00EE, 0xBACC, 0xFF00, 0x0001 }) {
	: _device(Device::Config{ FILE_WRITE_DATA, 0x00FF, 0xBACC, 0xFF00, 0x0001 }) { //hawku
	memset(_buf, 0, sizeof(_buf));
	Config config;
	memcpy(_buf, &config, sizeof(Config));
}

Vmulti::~Vmulti(void) noexcept {
}