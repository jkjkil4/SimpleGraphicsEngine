#pragma once

#include <thread>
#include "utility_dx.h"
#include "header.h"
#include "Wait_Notify.h"
#include "Counter.h"
#include "Error.h"
#include "Object.h"
#include "Point.h"

namespace ge {
	class Window;
	class Image_dx;

	class Device_dx : public Object
	{
		/*
			��Ҫ��Reset�豸ʱ����Ķ���
			g_pSprite g_pSpriteRender
			g_pRenderTexture g_pRenderSurface g_pWindowSurface

			��Ҫ������ʱRelease�Ķ���
			g_pDevice
			g_pSprite g_pSpriteRender
			g_pRenderTexture g_pRenderSurface g_pWindowSurface
		*/
	public:
		Device_dx(Window* wnd);
		~Device_dx() override;

		void gloDestroy();

		void begin();	//��ʼ������
		void end();		//����������

		void forceResetDevice();
		void resetDevice();
		void updateScale();

		void drawImage(const Image_dx& image, const PointF& pos, D3DCOLOR blendColor = D3DCOLOR_XRGB(255, 255, 255));	//����ͼ��

		void drawRect(int x, int y, int w, int h, DWORD col);
		void drawRect(int x, int y, int w, int h, DWORD col1, DWORD col2, DWORD col3, DWORD col4);
		void drawRect(RECT rect, DWORD col);
		void drawRect(RECT rect, DWORD col1, DWORD col2, DWORD col3, DWORD col4);
		void drawBorder(int x, int y, int w, int h, int size, DWORD col = 0xff000000);

		void updatePresentParameters();		//���ݴ��ڴ�С����d3dpp

		Signal<ObjFn> releasingDevice;	//��release�豸ʱ�������ź�
		Signal<ObjFn> resettingDevice;	//��reset�豸ʱ�������ź�

		VAR_FUNC(BackgroundColor, backgroundColor, D3DCOLOR,,)
		VAR_FUNC(GlobalBlend, globalBlend, D3DCOLOR,,)
		VAR_NL_FUNC(IsResettingDevice, isResettingDevice, bool,,)

	private:
		friend class Image_dx;

		Wait_Notify wnDevice;
		std::thread* thDevice;
		void thDeviceFn();
		bool needExitThDevice = false;
		VAR_NL_FUNC(NeedExitThDevice, needExitThDevice, bool,,)

		bool isResettingDevice = false;
		bool needForceReset = false;
		VAR_NL_FUNC(NeedForceReset, needForceReset, bool,,)

		Window* wnd;
		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;	//d3d��ʾģʽ�ṹ��
		LPDevice g_pDevice = nullptr;	//dx�豸
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(0, 0, 0);		//clearʱ����ɫ
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);	//ȫ�ֻ�ɫ

		LPVertexBuffer vbRectangle;
		LPIndexBuffer ibRectangle;

		LPSprite g_pSprite;			//���ڻ��Ƶ�Sprite

		enum class RenderType { Device, Sprite } renderType = RenderType::Device;
		void checkRenderType(RenderType rt);

		void onLostDevice_RenderTexture();
		void onResetDevice_RenderTexture();
		LPSurface g_pWindowSurface = nullptr;	//����Surface
		LPTexture g_pRenderTexture = nullptr;	//������Ⱦ������� exture
		LPSurface g_pRenderSurface = nullptr;	//�����Surface
		LPSprite g_pSpriteRender = nullptr;	//������Ⱦ�������Sprite

		//�豸Lost��Reset���
		void onLostDevice();
		void onResetDevice();
	};
}
