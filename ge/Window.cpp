#include "Window.h"
#include "ge.h"
#include "Console.h"
#include "Event.h"

GE_NAMESPACE;

map<HWND, Window*> Window::mapWnd;
mutex Window::mtxMsg;
int Window::wndCount = 0;

Window::Window() {
	tmpWaitNotify = new Wait_Notify;
	thMsg = new thread(&Window::thMsgFn, this);
	tmpWaitNotify->wait();
	SafeDelete(tmpWaitNotify);
}

Window::~Window() {
	close();
	thMsg->join();
	SafeDelete(thMsg);
}

void Window::thMsgFn() {
	threadId = GetCurrentThreadId();

	className = _T("SGE") + std::to_wstring(wndCount);

	//窗口设定
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = SimpleGraphicsEngine::g_hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className.c_str();
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	wndCount++;

	//创建窗口
	g_hWnd = CreateWindow(wc.lpszClassName,
		_T("SGE"), WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL);
	

	//得到窗口画面的区域
	RECT rect;
	GetClientRect(g_hWnd, &rect);
	clientSize = Size(rect.right - rect.left, rect.bottom - rect.top);

	mapWnd[g_hWnd] = this;
	tmpWaitNotify->notify();

	//消息处理
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		GetMessage(&msg, 0, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	mtxMsg.lock();
	vector<Window*>& vSendQuitMsgHWnd = SimpleGraphicsEngine::vSendQuitMsgHWnd;
	vector<Window*>::iterator iter = std::find(vSendQuitMsgHWnd.begin(), vSendQuitMsgHWnd.end(), this);
	if(iter != vSendQuitMsgHWnd.end())
		vSendQuitMsgHWnd.erase(iter);
	mtxMsg.unlock();

	UnregisterClass(className.c_str(), SimpleGraphicsEngine::g_hInstance);
	wndCount--;
}


void Window::setWindowTitle(const wstring& title) {
	wndTitle = title;
	SetWindowText(g_hWnd, wndTitle.c_str());
}
std::wstring Window::getWindowTitle() const {
	return wndTitle;
}

Rect Window::rect() const {
	RECT rect;
	GetWindowRect(g_hWnd, &rect);
	return Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}
void Window::move(int x, int y, int w, int h) {
	MoveWindow(g_hWnd, x, y, w, h, true);
}
void Window::move(const Rect& rect) {
	MoveWindow(g_hWnd, rect.x, rect.y, rect.width, rect.height, true);
}
void Window::move(const Point& pos) {
	RECT rect;
	GetWindowRect(g_hWnd, &rect);
	MoveWindow(g_hWnd, pos.x, pos.y, rect.right - rect.left, rect.bottom - rect.top, true);
}
void Window::move(const Size& size) {
	RECT rect;
	GetWindowRect(g_hWnd, &rect);
	MoveWindow(g_hWnd, rect.left, rect.top, size.width, size.height, true);
}

void Window::show(int nCmdShow) {
	ShowWindow(g_hWnd, nCmdShow);
}
bool Window::isVisible() const {
	return IsWindowVisible(g_hWnd);
}
void Window::close() {
	SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}


LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	auto iter = mapWnd.find(hWnd);
	if (iter == mapWnd.end())
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	return (*iter).second->procWndMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::procWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_SIZE:
		//得到窗口画面的区域
		RECT rect;
		GetClientRect(g_hWnd, &rect);
		clientSize = Size(rect.right - rect.left, rect.bottom - rect.top);
		mtxMsg.lock();
		SimpleGraphicsEngine::postEvent(this, (EventFunc)&Window::resizeEvent, new ResizeEvent(clientSize), true);
		mtxMsg.unlock();
		break;
	case WM_CLOSE:
		//销毁窗口
		DestroyWindow(hWnd);
		mtxMsg.lock();
		mapWnd.erase(g_hWnd);
		mtxMsg.unlock();
		break;
	case WM_DESTROY:
		//退出消息循环
		mtxMsg.lock();
		SimpleGraphicsEngine::vSendQuitMsgHWnd.push_back(this);
		mtxMsg.unlock();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::resizeEvent(ResizeEvent* ev) { cout << "RESIZE" << endl; }