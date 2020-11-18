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

		//���ڱ������
		void setWindowTitle(const std::wstring& title);
		std::wstring getWindowTitle() const;

		//���ڵ�λ�úʹ�С���
		VAR_GET_FUNC(ClientSize, clientSize, Size)
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
		friend class Device_dx;

		Wait_Notify* tmpWaitNotify = nullptr;	//��ʱ�������ȴ���ʼ���ģ���ָ����Ϊ��������֮����ռ��̫���ڴ�

		HWND g_hWnd = nullptr;	//���ھ��
		std::wstring wndTitle = _T("SGE");	//���ڱ���
		Size clientSize;	//���ڻ��������

		//������Ϣ�������
		static std::map<HWND, Window*> mapWnd;	//������Ӧ (HWND - Window*) ��map���Ա���WndProc�е���procWndMessage
		static int wndCount;	//��������
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���õ���Ϣ������
		LRESULT CALLBACK procWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���Ե���Ϣ������
		std::thread* thMsg = nullptr;	//��Ϣ�����߳�
		DWORD threadId = 0;		//��Ϣ�����߳�id
		std::wstring className;	//���ڵ�className
		void thMsgFn();	//��Ϣ�����̵߳ĺ���

		//�¼����
		virtual void resizeEvent(ResizeEvent*) {}
		virtual void paintEvent(PaintEvent*) {}

		//�������
		void update();	//����paintEvent
	};
}
