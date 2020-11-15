#pragma once

#include "Object.h"

namespace ge {
	class Timer : public Object
	{
	public:
		Timer();
		~Timer() override;

		void start(double msec);	//开始定时器
		void stop();	//停止计时器

		void setSingleShot(bool ss);	//设置是否单次触发

		Signal<ObjFn> timeout;

	private:
		friend class SimpleGraphicsEngine;

		bool isStart = false;
		bool isSingleShot = false;
		double delayMsec;
		double lastMsec;
	};
}
