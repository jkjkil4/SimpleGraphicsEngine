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
			需要在Reset设备时管理的东西
			g_pSprite g_pSpriteRender
			g_pRenderTexture g_pRenderSurface g_pWindowSurface

			需要在销毁时Release的东西
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

		LPSprite g_pSprite;			//用于绘制的Sprite

		void onLostDevice_RenderTexture();
		void onResetDevice_RenderTexture();
		LPSurface g_pWindowSurface = nullptr;	//窗口Surface
		LPTexture g_pRenderTexture = nullptr;	//用于渲染到纹理的 exture
		LPSurface g_pRenderSurface = nullptr;	//纹理的Surface
		LPSprite g_pSpriteRender = nullptr;	//用于渲染到纹理的Sprite

		//设备Lost和Reset相关
		void onLostDevice();
		void onResetDevice();
	};
}
