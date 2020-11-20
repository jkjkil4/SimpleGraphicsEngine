#pragma once

#include <thread>
#include "utility_dx.h"
#include "header.h"
#include "Wait_Notify.h"
#include "Counter.h"
#include "Error.h"
#include "Object.h"

namespace ge {
	class Window;

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

		void begin();
		void end();

		void drawTexture();

		void updatePresentParameters();

		VAR_FUNC(BackgroundColor, backgroundColor, D3DCOLOR,,)
		VAR_FUNC(GlobalBlend, globalBlend, D3DCOLOR,,)

	private:
		friend class StaticImage;

		Wait_Notify wnDevice;
		std::thread* thDevice;
		void thDeviceFn();
		bool needExitThDevice = false;
		VAR_NL_FUNC(NeedExitThDevice, needExitThDevice, bool,,)

		void resetDevice();
		std::condition_variable cvReset;
		std::mutex mtxReset;
		bool needResetDevice = false;

		Window* wnd;
		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDevice g_pDevice;
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(255, 255, 255);
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);

		LPSprite g_pSprite;			//���ڻ��Ƶ�Sprite

		void onLostDevice_RenderTexture();
		void onResetDevice_RenderTexture();
		LPSurface g_pWindowSurface;	//����Surface
		LPTexture g_pRenderTexture;	//������Ⱦ������� exture
		LPSurface g_pRenderSurface;	//�����Surface
		LPSprite g_pSpriteRender;	//������Ⱦ�������Sprite

		//�豸Lost��Reset���
		void onLostDevice();
		void onResetDevice();
	};
}
