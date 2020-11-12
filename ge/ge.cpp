#include "ge.h"
#include "Window.h"
#include "Delayer.h"
#include "Console.h"
#include "Event.h"

GE_NAMESPACE;

typedef SimpleGraphicsEngine SGE;
HINSTANCE SGE::g_hInstance = nullptr;
int SGE::delayMicro;
std::vector<Window*> SimpleGraphicsEngine::vSendQuitMsgHWnd;
std::list<SendEvent> SimpleGraphicsEngine::lSendEvent;


SimpleGraphicsEngine::SimpleGraphicsEngine(HINSTANCE hInstance, int _delayMicro)
{
#ifdef CONSOLE
	Console::alloc();
#endif

	g_hInstance = hInstance;
	delayMicro = _delayMicro;
}

Rect SimpleGraphicsEngine::screenAvailableRect() {
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	return Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

void SimpleGraphicsEngine::postEvent(Window* wnd, EventFunc func, Event* ev, bool single) {
	SendEvent se = SendEvent{ wnd, func, ev, single };
	if (se.single) {
		auto iter = find(lSendEvent.begin(), lSendEvent.end(), se);
		if (iter != lSendEvent.end())
			lSendEvent.erase(iter);
	}
	lSendEvent.push_back(se);
}

int SimpleGraphicsEngine::exec() {
	Counter counter;
	counter.start();
	double startTime = counter.getTime();
	Delayer delayer(delayMicro);

	while (Window::wndCount) {
		Window::mtxMsg.lock();

		//发送事件
		for (SendEvent& se : lSendEvent) {
			(se.wnd->*se.func)(se.ev);
			delete se.ev;
		}
		lSendEvent.clear();

		//使得需要退出消息循环的窗口退出消息循环
		for (Window* wnd : vSendQuitMsgHWnd)
			PostThreadMessage(wnd->threadId, WM_QUIT, 0, 0);

		Window::mtxMsg.unlock();

		//时间控制
		int spentMicro = (int)((counter.getTime() - startTime) * 1000);
		delayer.delay(delayMicro - spentMicro);
		cout << counter.getTime() - startTime << endl;
		startTime = counter.getTime();
	}

	cout << "Quit" << endl;
	Sleep(1000);

#ifdef CONSOLE
	Console::free();
#endif

	return 0;
}
