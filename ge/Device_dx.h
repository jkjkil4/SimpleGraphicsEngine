#pragma once

#include <thread>
#include "utility_dx.h"
#include "Device.h"
#include "vf.h"
#include "Wait_Notify.h"
#include "Counter.h"
#include "Error.h"

namespace ge {
	class Window;

	/*
		对dx的封装
		使用 gloDestroy() 全局销毁 (指Release了g_pD3D)
	*/
	class Device_dx : public Device
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

		static void gloDestroy();

		void begin() override;
		void end() override;

		void updatePresentParameters();

		VAR_FUNC(BackgroundColor, backgroundColor, D3DCOLOR,,)
		VAR_FUNC(GlobalBlend, globalBlend, D3DCOLOR,,)

	private:
		Wait_Notify wnDevice;
		std::thread* thDevice;
		void thDeviceFn();
		bool needExitThDevice = false;
		VAR_NL_FUNC(NeedExitThDevice, needExitThDevice, bool,,)

		void resetDevice();
		std::condition_variable cvReset;
		std::mutex mtxReset;
		bool needResetDevice = false;


		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDevice g_pDevice;
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(255, 255, 255);
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);

		LPSprite g_pSprite;			//用于绘制的Sprite

		void onLostDevice_RenderTexture();
		void onResetDevice_RenderTexture();
		LPSurface g_pWindowSurface;	//窗口Surface
		LPTexture g_pRenderTexture;	//用于渲染到纹理的 exture
		LPSurface g_pRenderSurface;	//纹理的Surface
		LPSprite g_pSpriteRender;	//用于渲染到纹理的Sprite

		//设备Lost和Reset相关
		void onLostDevice();
		void onResetDevice();
	};
}
