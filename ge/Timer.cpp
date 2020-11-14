#include "Timer.h"
#include "ge.h"

GE_NAMESPACE;

Timer::Timer() {
	//向SGE添加该对象的指针
	SGE::vTimers.push_back(this);
}

Timer::~Timer() {
	//从SGE移除该对象的指针
	auto iter = find(SGE::vTimers.begin(), SGE::vTimers.end(), this);
	if (iter != SGE::vTimers.end())
		SGE::vTimers.erase(iter);
}