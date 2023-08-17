#include "Service.h"

Service::Service(void) {
}

Service::~Service(void) {
	_run = false;
	_thread.join();
}

void Service::Start(void) {
	_run = true;
	_thread = std::thread(&Service::Run, this);
	SetThreadPriority(reinterpret_cast<HANDLE>(_thread.native_handle()), THREAD_PRIORITY_TIME_CRITICAL);
}
void Service::Run(void)  noexcept {

	while (true) {
		if (!_tablet->Read())
			continue;
		_mapper->Convert();
		_vmulti->Write();


	}
}

//#include <iostream>
//
//double avg = 0;
//long long idx = 0;
//clock_t start = clock();
//
//clock_t finish = clock();
//avg += finish - start;
//idx++;
//std::cout << avg / idx << std::endl;
//if (_vmulti->_report.button & 1) {
//	avg = 0;
//	idx = 0;
//}