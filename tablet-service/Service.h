#pragma once
#include <thread>
#include <mutex>
#include <atomic>

#include "Tablet.h"
#include "Vmulti.h"
#include "Mapper.h"

class Service final {
public:
	explicit Service(void);
	~Service(void);
public:
	void Start(void);
	void Run(void)  noexcept;
public:
	Tablet* _tablet = nullptr;
	Vmulti* _vmulti = nullptr;
	Mapper* _mapper = nullptr;
private:
	std::thread _thread;
	std::mutex _mutex;
	std::atomic<bool> _run = false;
};