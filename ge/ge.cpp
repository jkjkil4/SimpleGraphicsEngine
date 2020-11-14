﻿#include "ge.h"
#include "Window.h"
#include "Delayer.h"
#include "Console.h"
#include "Event.h"

GE_NAMESPACE;

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

	while (Window::wndCount) {
		mtxML.lock();

		//发送事件
		for (SendEvent& se : lSendEvent) {
			(se.wnd->*se.func)(se.ev);
			delete se.ev;
		}
		lSendEvent.clear();

		//使得需要退出消息循环的窗口退出消息循环
		for (Window* wnd : vSendQuitMsgHWnd)
			PostThreadMessage(wnd->threadId, WM_QUIT, 0, 0);

		mtxML.unlock();

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
