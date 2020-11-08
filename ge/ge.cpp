#include "ge.h"
#include "Window.h"
#include "Delayer.h"
#include "Console.h"

GE_NAMESPACE;

typedef SimpleGraphicsEngine SGE;
HINSTANCE SGE::g_hInstance = nullptr;
int SGE::delayMicro;
std::vector<Window*> SimpleGraphicsEngine::vSendQuitMsgHWnd;


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

int SimpleGraphicsEngine::exec() {
	Counter counter;
	counter.start();
	double startTime = counter.getTime();
	Delayer delayer(delayMicro);

	while (Window::wndCount) {
		Window::mtxMsg.lock();

		//TODO: MainLoop

		//使得需要退出消息循环的窗口退出消息循环
		for (Window* wnd : vSendQuitMsgHWnd) {
			PostThreadMessage(wnd->threadId, WM_QUIT, 0, 0);
		}

		Window::mtxMsg.unlock();


		//时间控制
		int spentMicro = (int)((counter.getTime() - startTime) * 1000);
		delayer.delay(delayMicro - spentMicro);
		startTime = counter.getTime();
	}

	cout << "Quit" << endl;
	Sleep(1000);

#ifdef CONSOLE
	Console::free();
#endif

	return 0;
}
