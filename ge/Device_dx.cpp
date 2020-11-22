#include "Device_dx.h"
#include "Window.h"
#include "ge.h"
#include "Image_dx.h"

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
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//dx��ص��߳�
	thDevice = new thread(&Device_dx::thDeviceFn, this);
	wnDevice.wait();
	connect(&SGE::exited, this, (ObjFn)&Device_dx::gloDestroy);
}

Device_dx::~Device_dx() {
	setNeedExitThDevice(true);
	wnDevice.notify();
	thDevice->join();

	SafeRelease(g_pWindowSurface);
	SafeRelease(g_pRenderSurface);
	SafeRelease(g_pRenderTexture);
	SafeRelease(g_pSprite);
	SafeRelease(g_pSpriteRender);
	SafeRelease(g_pDevice);
}


void Device_dx::gloDestroy() {
	g_pD3D->Release();
	disconnectAll(&SGE::exited, (ObjFn)&Device_dx::gloDestroy);
}


void Device_dx::begin() {
	g_pSprite->End();
	g_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, backgroundColor, 1.0f, 0);	//��bgColor���
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Device_dx::end() {
	g_pSprite->End();	//����Sprite�Ļ���

	g_pDevice->SetRenderTarget(0, g_pWindowSurface);
	g_pDevice->BeginScene();	//��ȡ����Ȩ��
	g_pSpriteRender->Begin(0);
	g_pSpriteRender->Draw(g_pRenderTexture, nullptr, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), globalBlend);//��������Ƶ�����
	g_pSpriteRender->End();
	g_pDevice->EndScene();		//��������

	HRESULT hr = g_pDevice->Present(nullptr, nullptr, 0, nullptr);	//ǰ��̨����������
	if (FAILED(hr) && !wnd->getClosed()) {
		resetDevice();
		return;
	}
	g_pDevice->SetRenderTarget(0, g_pRenderSurface);
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	//��ʼSprite�Ļ���
}

void Device_dx::drawImage(const Image_dx& image, const PointF& pos, D3DCOLOR blendColor) {
	g_pSprite->Draw(image.g_pTexture, nullptr, nullptr, &D3DXVECTOR3(pos.x, pos.y, 0), blendColor);
}


void Device_dx::updatePresentParameters() {
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = wnd->clientSize.width;
	d3dpp.BackBufferHeight = wnd->clientSize.height;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//��תЧ��������
}


void Device_dx::thDeviceFn() {
	//�豸����
	updatePresentParameters();

	//�����豸ָ��
	g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,		//Ĭ���Կ�
		D3DDEVTYPE_HAL,			//Ӳ�������
		wnd->g_hWnd,					//�������Ĵ��ڣ�Ҫ����Ĵ��ڣ�
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//�����������ģʽ
		&d3dpp,		//�豸������
		&g_pDevice	//���ص��豸ָ��
	);

	//����Sprite
	D3DXCreateSprite(g_pDevice, &g_pSprite);
	D3DXCreateSprite(g_pDevice, &g_pSpriteRender);

	//��Ⱦ������ ���
	onResetDevice_RenderTexture();

	//�����̼߳���ִ��
	wnDevice.notify();

	//ѭ��ֱ����Ҫ�˳�
	while (true) {
		wnDevice.wait();
		if (getNeedExitThDevice())
			break;

		//����豸״̬
		HRESULT hr = g_pDevice->TestCooperativeLevel();

		//�豸�ܱ�Reset
		if (hr == D3DERR_DEVICENOTRESET) {
			Counter counter;
			counter.start();
			//double startTime = counter.getTime();
			onLostDevice();
			HRESULT hr = g_pDevice->Reset(&d3dpp);
			if (SUCCEEDED(hr)) {
				onResetDevice();
			}
			else {
				SGE::msgBox(_T("Error: Cannot Reset D3DDevice"));
				//throw Error(Error::ER_RESETFAILED, _T("Error: Cannot Reset D3DDevice"));
			}
		}
		else if (hr == D3DERR_DEVICELOST) {
			Sleep(25);
		}

		setIsResettingDevice(false);
	}
}

void Device_dx::resetDevice() {
	setIsResettingDevice(true);
	wnDevice.notify();
}


void Device_dx::onLostDevice() {
	EMIT(&releasingDevice);
	onLostDevice_RenderTexture();
	g_pSprite->OnLostDevice();
	g_pSpriteRender->OnLostDevice();
}

void Device_dx::onResetDevice() {
	EMIT(&resettingDevice);
	onResetDevice_RenderTexture();
	g_pSprite->OnResetDevice();
	g_pSpriteRender->OnResetDevice();
}

void Device_dx::onLostDevice_RenderTexture() {
	SafeRelease(g_pRenderSurface);
	SafeRelease(g_pRenderTexture);
	SafeRelease(g_pWindowSurface);
}

void Device_dx::onResetDevice_RenderTexture() {
	//����������Ⱦ�������Texture��Surface
	g_pDevice->CreateTexture(
		wnd->clientSize.width, wnd->clientSize.height, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &g_pRenderTexture, NULL
	);
	g_pRenderTexture->GetSurfaceLevel(0, &g_pRenderSurface);

	//�õ��豸��Surface
	g_pDevice->GetRenderTarget(0, &g_pWindowSurface);
	g_pDevice->SetRenderTarget(0, g_pRenderSurface);
}