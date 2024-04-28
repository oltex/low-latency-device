#include "Vmulti.h"

Vmulti::Vmulti(void)
	//: _device(Device::Config{ 0x00EE, 0xBACC, 0xFF00, 0x0001, FILE_WRITE_DATA }) {
	: _device(Device::Config{ 0x00FF, 0xBACC, 0xFF00, 0x0001, FILE_WRITE_DATA }) { //hawku
	memset(_buf, 0, sizeof(_buf));
	Vmulti::Config config;
	memcpy(_buf, &config, sizeof(Vmulti::Config));
}

Vmulti::~Vmulti(void) {
}

void Vmulti::Write(void) noexcept {
	//memcpy(_buf + 4, &_report, sizeof(Report));
	_device.Write(_buf, sizeof(_buf));
}