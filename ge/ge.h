#pragma once

#include <map>
#include <mutex>
#include <vector>
#include "utility.h"
#include "Rect.h"

namespace ge {
	class Window;

	class SimpleGraphicsEngine
	{
	public:
		SimpleGraphicsEngine(HINSTANCE hInstance, int _delayMicro = 16667);
		~SimpleGraphicsEngine() = default;

		static Rect screenAvailableRect();

		static int exec();

	private:
		friend class Window;

		static HINSTANCE g_hInstance;
		static int delayMicro;	//延时时间

		static std::vector<Window*> vSendQuitMsgHWnd;	//记录需要退出消息循环的窗口
	};
}
