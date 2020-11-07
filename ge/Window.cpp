#include "Window.h"
#include "ge.h"

GE_NAMESPACE;

Window::Window() {
	className = _T("SGE") + std::to_wstring(SimpleGraphicsEngine::ins->wndCount);

	//窗口设定
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = SimpleGraphicsEngine::ins->g_hInstance;
	wc.lpfnWndProc = SimpleGraphicsEngine::WndProc;
	wc.lpszClassName = className.c_str();
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	//创建窗口
	g_hWnd = CreateWindow(wc.lpszClassName,
		_T("SGE"), WS_OVERLAPPEDWINDOW, 40, 20,
		GetSystemMetrics(SM_CXFULLSCREEN) - 80, GetSystemMetrics(SM_CYFULLSCREEN) - 40,
		NULL, NULL, wc.hInstance, NULL);

	//最大化
	ShowWindow(g_hWnd, SW_SHOWMAXIMIZED);

	//得到画面宽高
	RECT rect;
	GetClientRect(g_hWnd, &rect);
	defWidth = viewWidth = rect.right - rect.left;
	defHeight = viewHeight = rect.bottom - rect.top;
}

Window::~Window() {
	cout << "c" << endl;
	SendMessage(g_hWnd, WM_CLOSE, 0, 0);
	cout << "a" << endl;
}

LRESULT CALLBACK Window::ProcWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		UnregisterClass(className.c_str(), SimpleGraphicsEngine::ins->g_hInstance);
		SimpleGraphicsEngine::ins->mapWnd.erase(g_hWnd);
		cout << "b" << endl;
		break;
	case WM_DESTROY:
		SimpleGraphicsEngine::ins->wndCount--;
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}