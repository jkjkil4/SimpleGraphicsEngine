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

		void begin();	//开始主绘制
		void end();		//结束主绘制

		void forceResetDevice();
		void resetDevice();
		void updateScale();

		void drawImage(const Image_dx& image, const PointF& pos, D3DCOLOR blendColor = D3DCOLOR_XRGB(255, 255, 255));	//绘制图像

		void drawRect(int x, int y, int w, int h, DWORD col);
		void drawRect(int x, int y, int w, int h, DWORD col1, DWORD col2, DWORD col3, DWORD col4);
		void drawRect(RECT rect, DWORD col);
		void drawRect(RECT rect, DWORD col1, DWORD col2, DWORD col3, DWORD col4);
		void drawBorder(int x, int y, int w, int h, int size, DWORD col = 0xff000000);

		void updatePresentParameters();		//根据窗口大小更新d3dpp

		Signal<ObjFn> releasingDevice;	//当release设备时发出的信号
		Signal<ObjFn> resettingDevice;	//当reset设备时发出的信号

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

		D3DPRESENT_PARAMETERS d3dpp;	//d3d显示模式结构体
		LPDevice g_pDevice = nullptr;	//dx设备
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(0, 0, 0);		//clear时的颜色
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);	//全局混色

		LPVertexBuffer vbRectangle;
		LPIndexBuffer ibRectangle;

		LPSprite g_pSprite;			//用于绘制的Sprite

		enum class RenderType { Device, Sprite } renderType = RenderType::Device;
		void checkRenderType(RenderType rt);

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
