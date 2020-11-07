#include "ge.h"

#include "Console.h"
#include "Window.h"

GE_NAMESPACE;

SimpleGraphicsEngine* SimpleGraphicsEngine::ins = nullptr;


SimpleGraphicsEngine::SimpleGraphicsEngine(HINSTANCE hInstance, int delayMicro)
	: g_hInstance(hInstance), delayMicro(delayMicro)
{
	if (ins)
		throw;
	ins = this;

#ifdef CONSOLE
	Console::alloc();
#endif

	cout << "Will Start Thead Wnd" << endl;
	thWnd = new thread(&SimpleGraphicsEngine::thWndFn, this);	//窗口初始化和消息处理的线程
	cout << "Thread Wnd Started" << endl;

	cout << "Will Start Thread Dx" << endl;
	thDx = new thread(&SimpleGraphicsEngine::thDxFn, this);	//dx初始化和重置设备的线程
	cvWait(cv);
	cout << "Thread Dx Started" << endl;
}

Window* SimpleGraphicsEngine::createWindow() {
	cout << "Will Create Window" << endl;
	Window* wnd = nullptr;
	if (!getIsNeedQuit()) {
		PostThreadMessage(msgThreadId, WMU_CREATE_WND, 0, (LPARAM)&wnd);
		cvWait(cv);
		cout << "Created Window" << wnd << endl;
	}
	return wnd;
}
//void SimpleGraphicsEngine::removeWindow(Window* wnd) {
//	PostThreadMessage(msgThreadId, WMU_REMOVE_WND, 0, (LPARAM)wnd);
//	cvWait(cv);
//	mapWnd.erase(wnd->g_hWnd);
//}

void SimpleGraphicsEngine::cvWait(condition_variable& cv) {
	unique_lock<std::mutex> lck(mutexCv);
	if (!cvFlag)
		cv.wait(lck);
	cvFlag = false;
}
void SimpleGraphicsEngine::cvNotify(condition_variable& cv) {
	unique_lock<std::mutex> lck(mutexCv);
	cvFlag = true;
	cv.notify_all();
}

void SimpleGraphicsEngine::initDx() {
	//创建D3D接口指针
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	//显示模式结构体
	ZeroMemory(&d3ddm, sizeof(d3ddm));

	//获取当前显卡的显示模式
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	////设置d3dpp
	//d3dpp.BackBufferCount = 1;
	//d3dpp.Windowed = TRUE;
	//d3dpp.BackBufferWidth = getDefWidth();
	//d3dpp.BackBufferHeight = getDefHeight();
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//翻转效果：抛弃

	////创建设备指针
	//g_pD3D->CreateDevice(
	//	D3DADAPTER_DEFAULT,		//默认显卡
	//	D3DDEVTYPE_HAL,			//硬件抽象层
	//	g_hWnd,					//所依附的窗口（要改造的窗口）
	//	D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//顶点软件处理模式
	//	&d3dpp,		//设备的能力
	//	&g_pDevice	//返回的设备指针
	//	);
}

void SimpleGraphicsEngine::thWndFn() {
	msgThreadId = GetCurrentThreadId();

	//接收消息
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (true) {
		GetMessage(&msg, 0, 0, 0);
		if (msg.message == WM_QUIT) {
			cout << "QUIT" << endl;
			break;
		}
		if (msg.message == WMU_CREATE_WND) {
			cout << "CREATE" << endl;
			wndCount++;
			Window** pWnd = (Window**)msg.lParam;
			(*pWnd) = new Window;
			mapWnd[(*pWnd)->g_hWnd] = *pWnd;
			cvNotify(cv);
			continue;
		}
		mutexMsg.lock();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		mutexMsg.unlock();
	}

	//设置需要退出
	setIsNeedQuit(true);
}

void SimpleGraphicsEngine::thDxFn() {
	//initDx();
	cvNotify(cv);
	while (!getIsNeedQuit()) {
		if (getIsDxNeedReset()) {
			//TODO: 重置设备
		}
		Sleep(10);
	}
}

LRESULT CALLBACK SimpleGraphicsEngine::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	auto iter = ins->mapWnd.find(hWnd);
	if (iter == ins->mapWnd.end())
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	return (*iter).second->ProcWndMessage(hWnd, uMsg, wParam, lParam);
}

int SimpleGraphicsEngine::exec() {
	Counter counter;	//计时器
	counter.start();
	double startTime = counter.getTime();
	Delayer delayer(delayMicro);

	while (!getIsNeedQuit()) {
		mutexMsg.lock();
		//TODO: 主循环
		if (SimpleGraphicsEngine::ins->wndCount <= 0)
			PostThreadMessage(msgThreadId, WM_QUIT, 0, 0);
		mutexMsg.unlock();
		
		//TODO: device -> present

		//时间控制
		int spentMicro = (int)((counter.getTime() - startTime) * 1000);
		delayer.delay(delayMicro - spentMicro);
		startTime = counter.getTime();
	}

	cout << "quit" << endl;
	thDx->join();
	thWnd->join();
	for (auto& pair : mapWnd)
		delete pair.second;
	Sleep(2000);

#ifdef CONSOLE
	Console::free();
#endif

	return 0;
}