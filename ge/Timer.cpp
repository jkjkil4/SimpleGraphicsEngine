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

void Timer::start(double msec) {
	if (msec <= 0) {	//�����ʱʱ��С����0����ֱ�Ӵ���
		isStart = false;
		EMIT(&timeout);
	}
	else {	//�����ʱʱ�����0���ͽ���SGE����
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
