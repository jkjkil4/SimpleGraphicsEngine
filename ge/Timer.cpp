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

void Timer::start(double msec) {
	if (msec <= 0) {	//如果定时时间小等与0，就直接触发
		isStart = false;
		EMIT(&timeout);
	}
	else {	//如果定时时间大于0，就交给SGE处理
		isStart = true;
		delayMsec = msec;
		lastMsec = msec;
	}
}
void Timer::stop() {
	isStart = false;
}
void Timer::setSingleShot(bool ss) {
	isSingleShot = ss;
}
