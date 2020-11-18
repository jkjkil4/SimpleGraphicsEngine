#pragma once

#include <thread>
#include <map>
#include <string>
#include "utility.h"
#include "Wait_Notify.h"
#include "header.h"
#include "Object.h"
#include "Rect.h"
#include "Point.h"
#include "Size.h"

namespace ge {
	struct ResizeEvent;
	struct PaintEvent;
	class Device;

	class Window : public Object
	{
	public:
		Window();
		~Window() override;

		//窗口标题相关
		void setWindowTitle(const std::wstring& title);
		std::wstring getWindowTitle() const;

		//窗口的位置和大小相关
		VAR_GET_FUNC(ClientSize, clientSize, Size)
		Rect rect() const;
		void move(int x, int y, int w, int h);
		void move(const Rect& rect);
		void move(const Point& pos);
		void move(const Size& size);

		//窗口显示相关
		void show(int nCmdShow);
		bool isVisible() const;

		//关闭窗口
		void close();

	private:
		friend class SimpleGraphicsEngine;
		friend class Device_dx;

		Wait_Notify* tmpWaitNotify = nullptr;	//临时的用来等待初始化的，用指针是为了在用完之后不再占用太多内存

		HWND g_hWnd = nullptr;	//窗口句柄
		std::wstring wndTitle = _T("SGE");	//窗口标题
		Size clientSize;	//窗口画面的区域

		//窗口消息处理相关
		static std::map<HWND, Window*> mapWnd;	//用来对应 (HWND - Window*) 的map，以便在WndProc中调用procWndMessage
		static int wndCount;	//窗口数量
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//公用的消息处理函数
		LRESULT CALLBACK procWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//各自的消息处理函数
		std::thread* thMsg = nullptr;	//消息处理线程
		DWORD threadId = 0;		//消息处理线程id
		std::wstring className;	//窗口的className
		void thMsgFn();	//消息处理线程的函数

		//事件相关
		virtual void resizeEvent(ResizeEvent*) {}
		virtual void paintEvent(PaintEvent*) {}

		//绘制相关
		void update();	//发送paintEvent
	};
}
