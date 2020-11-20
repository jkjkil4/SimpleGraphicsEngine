#pragma once

#include "utility_dx.h"

namespace ge {
	class Device_dx;

	class StaticImage
	{
	public:
		enum LoadMethod { LM_File, LM_Resource };

		StaticImage(Device_dx& d, const std::wstring& path, LoadMethod loadMethod, UINT w = D3DX_FROM_FILE, UINT h = D3DX_FROM_FILE);

	private:
		LPTexture g_pTexture;
		ImageInfo imageInfo;
	};
}
