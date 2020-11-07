#pragma once

#include "utility.h"
#include "Wait_Notify.h"
#include <thread>
#include <map>
#include <string>
#include "Rect.h"
#include "Point.h"
#include "Size.h"

namespace ge {
	class Window
	{
	public:
		Window();
		~Window();

		//���ڱ������
		void setWindowTitle(const std::wstring& title);
		std::wstring getWindowTitle() const;

		//���ڵ�λ�úʹ�С���
		Rect rect() const;
		void move(int x, int y, int w, int h);
		void move(const Rect& rect);
		void move(const Point& pos);
		void move(const Size& size);

		//������ʾ���
		void show(int nCmdShow);
		bool isVisible() const;

		//�رմ���
		void close();

	private:
		friend class SimpleGraphicsEngine;

		Wait_Notify* tmpWaitNotify = nullptr;	//��ʱ�������ȴ���ʼ���ģ���ָ����Ϊ��������֮����ռ��̫���ڴ�

		HWND g_hWnd = nullptr;	//���ھ��
		std::wstring wndTitle = _T("SGE");	//���ڱ���

		//������Ϣ�������
		static std::map<HWND, Window*> mapWnd;	//������Ӧ (HWND - Window*) ��map���Ա���WndProc�е���procWndMessage
		static std::mutex mtxMsg;	//��Ϣ����ʱ���߳���
		static int wndCount;	//��������
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���õ���Ϣ������
		LRESULT CALLBACK procWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���Ե���Ϣ������
		std::thread* thMsg = nullptr;	//��Ϣ�����߳�
		DWORD threadId = 0;		//��Ϣ�����߳�id
		std::wstring className;	//���ڵ�className
		void thMsgFn();	//��Ϣ�����̵߳ĺ���
	};
}
