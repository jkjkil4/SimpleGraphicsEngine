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

		void begin();
		void end();

		void drawImage(const Image_dx& image, const PointF& pos, D3DCOLOR blendColor = D3DCOLOR_XRGB(255, 255, 255));

		void updatePresentParameters();

		Signal<ObjFn> releasingDevice;
		Signal<ObjFn> resettingDevice;

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

		void resetDevice();
		bool isResettingDevice = false;

		Window* wnd;
		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDevice g_pDevice = nullptr;
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(0, 0, 0);
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);

		LPSprite g_pSprite;			//���ڻ��Ƶ�Sprite

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
