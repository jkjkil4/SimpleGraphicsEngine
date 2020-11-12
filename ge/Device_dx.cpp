#include "Device_dx.h"
#include "Window.h"

GE_NAMESPACE;

LPDirectx Device_dx::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
D3DDISPLAYMODE Device_dx::d3ddm = ([](LPDirectx g_pD3D) -> D3DDISPLAYMODE {
	D3DDISPLAYMODE d3ddm;		//D3D��ʾģʽ�ṹ��
	ZeroMemory(&d3ddm, sizeof(d3ddm));
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm); 
	return d3ddm;
	})(g_pD3D);

Device_dx::Device_dx(Window* wnd) : wnd(wnd)
{
	//�豸����
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	updatePresentParameters();

	//�����豸ָ��
	g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,		//Ĭ���Կ�
		D3DDEVTYPE_HAL,			//Ӳ�������
		wnd->g_hWnd,					//�������Ĵ��ڣ�Ҫ����Ĵ��ڣ�
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//������������ģʽ
		&d3dpp,		//�豸������
		&g_pDevice	//���ص��豸ָ��
	);

	//����Sprite
	D3DXCreateSprite(g_pDevice, &g_pSprite);
	D3DXCreateSprite(g_pDevice, &g_pSpriteRender);

	//��Ⱦ������ ���

}


void Device_dx::begin() {
	g_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, backgroundColor, 1.0f, 0);
}

void Device_dx::end() {
	g_pSprite->End();	//����Sprite�Ļ���

	g_pDevice->BeginScene();	//��ȡ����Ȩ��
	g_pSpriteRender->Begin(0);
	g_pSpriteRender->Draw(g_pRenderTexture, nullptr, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), globalBlend);
	g_pSpriteRender->End();
	g_pDevice->EndScene();		//��������

	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	//��ʼSprite�Ļ���
}


void Device_dx::updatePresentParameters() {
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = wnd->clientSize.width;
	d3dpp.BackBufferHeight = wnd->clientSize.height;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//��תЧ��������
}


void Device_dx::onRelease_RenderTexture() {
	SafeRelease(g_pRenderSurface);
	SafeRelease(g_pRenderTexture);
	SafeRelease(g_pWindowSurface);
}

void Device_dx::onReset_RenderTexture() {
	//����������Ⱦ��������Texture��Surface
	g_pDevice->CreateTexture(
		wnd->clientSize.width, wnd->clientSize.height, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &g_pRenderTexture, NULL
	);
	g_pRenderTexture->GetSurfaceLevel(0, &g_pRenderSurface);

	//�õ��豸��Surface
	g_pDevice->GetRenderTarget(0, &g_pWindowSurface);
	g_pDevice->SetRenderTarget(0, g_pRenderSurface);
}