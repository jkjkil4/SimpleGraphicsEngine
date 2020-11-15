#pragma once

#include "Object.h"

namespace ge {
	class Timer : public Object
	{
	public:
		Timer();
		~Timer() override;

		void start(double msec);	//��ʼ��ʱ��
		void stop();	//ֹͣ��ʱ��

		void setSingleShot(bool ss);	//�����Ƿ񵥴δ���

		Signal<ObjFn> timeout;

	private:
		friend class SimpleGraphicsEngine;

		bool isStart = false;
		bool isSingleShot = false;
		double delayMsec;
		double lastMsec;
	};
}
