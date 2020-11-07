#pragma once

#include <thread>
#include <list>
#include <map>
#include <condition_variable>

#include "utility.h"
#include "vf.h"
#include "Delayer.h"

namespace ge {
	class Window;

	class SimpleGraphicsEngine
	{
	public:
		SimpleGraphicsEngine(HINSTANCE hInstance, int delayMicro = 16667);
		~SimpleGraphicsEngine() = default;

		Window* createWindow();

		bool isReseting() { return isDxNeedReset; }	//返回是否在重置设备
		int exec();

		VAR_GET_FUNC(DelayMicro, delayMicro, int)

	private:
		friend class Window;

		static SimpleGraphicsEngine* ins;		//初始化在ge.cpp
		//void removeWindow(Window* wnd);
		std::map<HWND, Window*> mapWnd;

		//传入的值
		HINSTANCE g_hInstance = nullptr;
		int delayMicro = 16667;

		//线程的等待和唤醒
		std::condition_variable cv;
		std::mutex mutexCv;
		bool cvFlag = false;
		void cvWait(std::condition_variable& cv);
		void cvNotify(std::condition_variable& cv);

		//和窗口消息处理相关的东西
		int wndCount = 0;
		DWORD msgThreadId;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::thread* thWnd = nullptr;
		void thWndFn();
		std::mutex mutexMsg;
		std::list<MSG> lMsgs;

		//和dx相关的东西
		D3DPRESENT_PARAMETERS d3dpp;	//描述D3D设备的能力
		D3DDISPLAYMODE d3ddm;		//D3D显示模式结构体
		LPDirectx g_pD3D = nullptr;		//D3D的接口指针，为了创建设备指针
		LPDevice g_pDevice = nullptr;	//D3D的设备指针，为了创建精灵指针

		//和dx初始化和重置设备相关的东西
		void initDx();
		std::thread* thDx = nullptr;	//dx初始化和重置的线程
		void thDxFn();					//该线程的函数
		bool isDxNeedReset = false;			//dx是否需要重置设备
		bool isNeedQuit = false;			//dx是否需要退出
		VAR_NL_FUNC(IsDxNeedReset, isDxNeedReset, bool,,)
		VAR_NL_FUNC(IsNeedQuit, isNeedQuit, bool,,)
	};
}
