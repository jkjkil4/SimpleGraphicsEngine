#include "StaticImage.h"
#include "Device_dx.h"

GE_NAMESPACE;

StaticImage::StaticImage(Device_dx& d, const std::wstring& path, LoadMethod loadMethod, UINT w, UINT h) {
	/*D3DXCreateTexture(d.g_pDevice, w, h, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &e->g_pRenderTexture, NULL
	);*/
	if (loadMethod == LM_File) {
		D3DXCreateTextureFromFileEx(
			d.g_pDevice,
			path.c_str(),
			w, h,
			0, 0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL,
			&imageInfo,
			nullptr,
			&g_pTexture
		);
	}
	else {

	}
}