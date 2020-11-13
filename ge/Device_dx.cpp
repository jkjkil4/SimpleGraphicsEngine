#include "Device_dx.h"
#include "Window.h"
#include "ge.h"

GE_NAMESPACE;

LPDirectx Device_dx::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
D3DDISPLAYMODE Device_dx::d3ddm = ([](LPDirectx g_pD3D) -> D3DDISPLAYMODE {
	D3DDISPLAYMODE d3ddm;		//D3D显示模式结构体
	ZeroMemory(&d3ddm, sizeof(d3ddm));
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm); 
	return d3ddm;
	})(g_pD3D);

Device_dx::Device_dx(Window* wnd) : Device(wnd)
{
	//dx相关的线程
	thDevice = new thread(&Device_dx::thDeviceFn, this);
	wnDevice.wait();
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
}


void Device_dx::begin() {
	g_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, backgroundColor, 1.0f, 0);	//用bgColor填充
}

void Device_dx::end() {
	g_pSprite->End();	//结束Sprite的绘制

	g_pDevice->BeginScene();	//获取绘制权限
	g_pSpriteRender->Begin(0);
	g_pSpriteRender->Draw(g_pRenderTexture, nullptr, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), globalBlend);//将纹理绘制到窗口
	g_pSpriteRender->End();
	g_pDevice->EndScene();		//结束绘制

	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	//开始Sprite的绘制
}


void Device_dx::updatePresentParameters() {
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = wnd->clientSize.width;
	d3dpp.BackBufferHeight = wnd->clientSize.height;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//翻转效果：抛弃
}


void Device_dx::thDeviceFn() {
	//设备属性
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	updatePresentParameters();

	//创建设备指针
	g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,		//默认显卡
		D3DDEVTYPE_HAL,			//硬件抽象层
		wnd->g_hWnd,					//所依附的窗口（要改造的窗口）
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//顶点软件处理模式
		&d3dpp,		//设备的能力
		&g_pDevice	//返回的设备指针
	);

	//创建Sprite
	D3DXCreateSprite(g_pDevice, &g_pSprite);
	D3DXCreateSprite(g_pDevice, &g_pSpriteRender);

	//渲染到纹理 相关
	onResetDevice_RenderTexture();

	//让主线程继续执行
	wnDevice.notify();

	//循环直到需要退出
	while (true) {
		wnDevice.wait();
		if (getNeedExitThDevice())
			break;

		//检查设备状态
		HRESULT hr = g_pDevice->TestCooperativeLevel();

		//设备能被Reset
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
				SimpleGraphicsEngine::msgBox(_T("Error: Cannot Reset D3DDevice"));
				//throw Error(Error::ER_RESETFAILED, _T("Error: Cannot Reset D3DDevice"));
			}
		}
		else if (hr == D3DERR_DEVICELOST) {
			Sleep(25);
		}

		onLostDevice();
		g_pDevice->Reset(&d3dpp);
		onResetDevice();
	}
}

void Device_dx::resetDevice() {
	wnDevice.notify();
}


void Device_dx::onLostDevice() {
	onLostDevice_RenderTexture();
	g_pSprite->OnLostDevice();
	g_pSpriteRender->OnLostDevice();
}

void Device_dx::onResetDevice() {
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
	//创建用于渲染到纹理的Texture和Surface
	g_pDevice->CreateTexture(
		wnd->clientSize.width, wnd->clientSize.height, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &g_pRenderTexture, NULL
	);
	g_pRenderTexture->GetSurfaceLevel(0, &g_pRenderSurface);

	//得到设备的Surface
	g_pDevice->GetRenderTarget(0, &g_pWindowSurface);
	g_pDevice->SetRenderTarget(0, g_pRenderSurface);
}