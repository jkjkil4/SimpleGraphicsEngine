#include "Timer.h"
#include "ge.h"

GE_NAMESPACE;

Timer::Timer() {
	//��SGE��Ӹö����ָ��
	SGE::vTimers.push_back(this);
}

Timer::~Timer() {
	//��SGE�Ƴ��ö����ָ��
	auto iter = find(SGE::vTimers.begin(), SGE::vTimers.end(), this);
	if (iter != SGE::vTimers.end())
		SGE::vTimers.erase(iter);
}