#include "Image_dx.h"
#include "Device_dx.h"

GE_NAMESPACE;


Image_dx::Image_dx(Device_dx& d, UINT w, UINT h, HRESULT* hr)
	: d(&d), type(Target)
{
	HRESULT tmpHr = d.g_pDevice->CreateTexture(w, h, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &g_pTexture, NULL);
	if (hr) *hr = tmpHr;
	size = Size(w, h);
	checkType();
}

Image_dx::Image_dx(Device_dx& d, const std::wstring& path, UINT w, UINT h, Type type, HRESULT* hr)
	: d(&d), type(type)
{
	ImageInfo info;
	HRESULT tmpHr = D3DXCreateTextureFromFileEx(
		d.g_pDevice,
		path.c_str(),
		w, h, 0, GetUsage(type),
		D3DFMT_UNKNOWN, GetPool(type),
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL,
		&info, nullptr, &g_pTexture
		);
	if (hr) *hr = tmpHr;
	size = Size(info.Width, info.Height);
	checkType();
}

Image_dx::Image_dx(Device_dx& d, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT w, UINT h, Type type, HRESULT* hr)
	: d(&d), type(type)
{
	ImageInfo info;
	HRESULT tmpHr = D3DXCreateTextureFromResourceEx(
		d.g_pDevice,
		hSrcModule,
		pSrcResource,
		w, h, 0, GetUsage(type),
		D3DFMT_UNKNOWN, GetPool(type),
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, 
		&info, nullptr, &g_pTexture
		);
	if (hr) *hr = tmpHr;
	size = Size(info.Width, info.Height);
	checkType();
}

Image_dx::~Image_dx() {
	SafeRelease(g_pTexture);
}

void Image_dx::checkType() {
	if (type == Target) {
		connect(&d->releasingDevice, this, (ObjFn)&Image_dx::onReleaseDevice);
		connect(&d->resettingDevice, this, (ObjFn)&Image_dx::onResetDevice);
	}
}

void Image_dx::onReleaseDevice() {
	SafeRelease(g_pTexture);
}

void Image_dx::onResetDevice() {
	d->g_pDevice->CreateTexture(size.width, size.height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &g_pTexture, NULL);
}
