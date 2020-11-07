#pragma once

#include <map>
#include <mutex>
#include "utility.h"
#include "Rect.h"

namespace ge {
	class Window;

	class SimpleGraphicsEngine
	{
	public:
		SimpleGraphicsEngine(HINSTANCE hInstance, int _delayMicro = 16667);
		~SimpleGraphicsEngine() = default;

		Rect screenAvailableRect();

		static int exec();

	private:
		friend class Window;

		static HINSTANCE g_hInstance;
		static int delayMicro;
	};
}
