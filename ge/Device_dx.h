#pragma once

#include "utility_dx.h"
#include "Device.h"
#include "vf.h"

namespace ge {
	class Window;

	/*
		��Ҫ��Reset�豸ʱ����Ķ���
		g_pSprite g_pSpriteRender
		g_pRenderTexture g_pRenderSurface g_pWindowSurface
	*/
	class Device_dx : public Device
	{
	public:
		Device_dx(Window* wnd);
		~Device_dx() override = default;

		void begin() override;
		void end() override;

		void updatePresentParameters();

		VAR_FUNC(BackgroundColor, backgroundColor, D3DCOLOR,,)
		VAR_FUNC(GlobalBlend, globalBlend, D3DCOLOR,,)

	private:
		Window* wnd;

		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDevice g_pDevice;
		D3DCOLOR backgroundColor = D3DCOLOR_XRGB(255, 255, 255);
		D3DCOLOR globalBlend = D3DCOLOR_XRGB(255, 255, 255);

		LPSprite g_pSprite;			//���ڻ��Ƶ� Sprite

		void onLostDevice_RenderTexture();
		void onResetDevice_RenderTexture();
		LPSurface g_pWindowSurface;	//���� Surface
		LPTexture g_pRenderTexture;	//������Ⱦ������� Texture
		LPSurface g_pRenderSurface;	//����� Surface
		LPSprite g_pSpriteRender;	//������Ⱦ������� Sprite

		//�豸Lost��Reset���
		void onLostDevice();
		void onResetDevice();
	};
}
