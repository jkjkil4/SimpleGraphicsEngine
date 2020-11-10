#include "Device_dx.h"
#include "Window.h"

GE_NAMESPACE;

LPDirectx Device_dx::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
D3DDISPLAYMODE Device_dx::d3ddm = ([](LPDirectx g_pD3D) -> D3DDISPLAYMODE {
	D3DDISPLAYMODE d3ddm;		//D3D显示模式结构体
	ZeroMemory(&d3ddm, sizeof(d3ddm));
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm); 
	return d3ddm;
	})(g_pD3D);

Device_dx::Device_dx(Window* wnd) : wnd(wnd)
{

}