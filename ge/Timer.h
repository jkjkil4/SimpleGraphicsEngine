#pragma once

#include "Object.h"

namespace ge {
	class Timer : public Object
	{
	public:
		Timer();
		~Timer() override;

		Signal<ObjFn> timeout;

	private:
		friend class SimpleGraphicsEngine;

		bool isStart = false;
	};
}
