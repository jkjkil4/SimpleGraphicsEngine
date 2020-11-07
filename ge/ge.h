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

		bool isReseting() { return isDxNeedReset; }	//�����Ƿ��������豸
		int exec();

		VAR_GET_FUNC(DelayMicro, delayMicro, int)

	private:
		friend class Window;

		static SimpleGraphicsEngine* ins;		//��ʼ����ge.cpp
		//void removeWindow(Window* wnd);
		std::map<HWND, Window*> mapWnd;

		//�����ֵ
		HINSTANCE g_hInstance = nullptr;
		int delayMicro = 16667;

		//�̵߳ĵȴ��ͻ���
		std::condition_variable cv;
		std::mutex mutexCv;
		bool cvFlag = false;
		void cvWait(std::condition_variable& cv);
		void cvNotify(std::condition_variable& cv);

		//�ʹ�����Ϣ������صĶ���
		int wndCount = 0;
		DWORD msgThreadId;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::thread* thWnd = nullptr;
		void thWndFn();
		std::mutex mutexMsg;
		std::list<MSG> lMsgs;

		//��dx��صĶ���
		D3DPRESENT_PARAMETERS d3dpp;	//����D3D�豸������
		D3DDISPLAYMODE d3ddm;		//D3D��ʾģʽ�ṹ��
		LPDirectx g_pD3D = nullptr;		//D3D�Ľӿ�ָ�룬Ϊ�˴����豸ָ��
		LPDevice g_pDevice = nullptr;	//D3D���豸ָ�룬Ϊ�˴�������ָ��

		//��dx��ʼ���������豸��صĶ���
		void initDx();
		std::thread* thDx = nullptr;	//dx��ʼ�������õ��߳�
		void thDxFn();					//���̵߳ĺ���
		bool isDxNeedReset = false;			//dx�Ƿ���Ҫ�����豸
		bool isNeedQuit = false;			//dx�Ƿ���Ҫ�˳�
		VAR_NL_FUNC(IsDxNeedReset, isDxNeedReset, bool,,)
		VAR_NL_FUNC(IsNeedQuit, isNeedQuit, bool,,)
	};
}
