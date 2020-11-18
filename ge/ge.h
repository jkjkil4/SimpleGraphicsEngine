#pragma once

#include <map>
#include <mutex>
#include <vector>
#include <list>
#include "utility.h"
#include "Object.h"
#include "Rect.h"

namespace ge {
	class Object;
	class Window;
	class Timer;
	struct SendEvent;
	struct Event;
	typedef void(Window::* EventFunc)(Event*);

	class SimpleGraphicsEngine : public Object
	{
	public:
		SimpleGraphicsEngine(HINSTANCE hInstance, int _delayMicro = 16667);
		~SimpleGraphicsEngine() = default;

		static Rect screenAvailableRect();

		static void msgBox(const std::wstring& text, HWND hWnd = nullptr);
		static void msgBox(const std::wstring& title, const std::wstring& text, HWND hWnd = nullptr);

		static int exec();

		static Signal<ObjFn> exited;

	private:
		friend class Window;
		friend class Timer;

		static std::mutex mtxML;

		static HINSTANCE g_hInstance;
		static int delayMicro;	//延时时间

		static std::vector<Window*> vSendQuitMsgHWnd;	//记录需要退出消息循环的窗口
		static std::list<SendEvent> lSendEvent;		//需要调用的事件
		static void postEvent(Window* wnd, EventFunc func, Event* ev, bool single);	//发送事件

		static std::vector<Timer*> vTimers;		//所有定时器
	};

	typedef SimpleGraphicsEngine SGE;
}
