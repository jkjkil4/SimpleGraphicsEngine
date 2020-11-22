#include "ge.h"
#include "Window.h"
#include "Delayer.h"
#include "Console.h"
#include "Event.h"
#include "Timer.h"

GE_NAMESPACE;

SGE::Signal<SGE::ObjFn> SGE::exited;

mutex SGE::mtxML;
HINSTANCE SGE::g_hInstance = nullptr;
int SGE::delayMicro;
vector<Window*> SGE::vSendQuitMsgHWnd;
list<SendEvent> SGE::lSendEvent;
vector<Timer*> SGE::vTimers;


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

void SimpleGraphicsEngine::msgBox(const std::wstring& text, HWND hWnd) {
	MessageBox(hWnd, text.c_str(), _T(""), MB_OK);
}
void SimpleGraphicsEngine::msgBox(const std::wstring& title, const std::wstring& text, HWND hWnd) {
	MessageBox(hWnd, text.c_str(), title.c_str(), MB_OK);
}

int SimpleGraphicsEngine::exec() {
	Counter counter;
	counter.start();
	double startTime = counter.getTime();
	Delayer delayer(delayMicro);

	double timerMsec = 0;

	while (Window::wndCount) {
		mtxML.lock();

		//发送事件
		for (SendEvent& se : lSendEvent) {
			(se.wnd->*se.func)(se.ev);
			delete se.ev;
		}
		lSendEvent.clear();

		//调用定时器
		for (Timer* timer : vTimers) {
			if (timer->isStart) {
				timer->lastMsec -= timerMsec;
				if (timer->lastMsec <= 0) {	//如果达到定时时间
					if (timer->isSingleShot) {	//如果为单次触发，则停止计时器并触发timeout
						timer->stop();
						EMIT(&timer->timeout);
					}
					else {	//如果不为单次触发
						while (timer->lastMsec <= 0) {
							timer->lastMsec += timer->delayMsec;
							EMIT(&timer->timeout);
						}
					}
				}
			}
		}

		//使得需要退出消息循环的窗口退出消息循环
		for (Window* wnd : vSendQuitMsgHWnd)
			PostThreadMessage(wnd->threadId, WM_QUIT, 0, 0);

		mtxML.unlock();

		//时间控制
		int spentMicro = (int)((counter.getTime() - startTime) * 1000);
		delayer.delay(delayMicro - spentMicro);

		double curTime = counter.getTime();
		timerMsec = curTime - startTime;
		startTime = curTime;
	}

	cout << "Quit" << endl;
	Sleep(1000);

	EMIT(&exited);

#ifdef CONSOLE
	Console::free();
#endif

	return 0;
}
