#pragma once

#include "utility.h"

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

namespace ge {
	typedef LPDIRECT3DTEXTURE9 LPTexture;
	typedef LPDIRECT3DSURFACE9 LPSurface;
	typedef LPD3DXSPRITE LPSprite;
	typedef LPD3DXFONT LPFont;
	typedef LPDIRECT3DVERTEXSHADER9 LPVertexShader;
	typedef LPDIRECT3DPIXELSHADER9 LPPixelShader;
	typedef LPD3DXCONSTANTTABLE LPConstantTable;

	typedef LPDIRECT3DVERTEXBUFFER9 LPVertexBuffer;
	typedef LPDIRECT3DINDEXBUFFER9 LPIndexBuffer;

	typedef LPDIRECT3D9 LPDirectx;
	typedef LPDIRECT3DDEVICE9 LPDevice;

	typedef D3DXIMAGE_INFO ImageInfo;

	template<typename T>inline void SafeRelease(T*& t) {
		if (t) {
			t->Release();
			t = nullptr;
		}
	}

	struct Vertex
	{
		float x, y, z, rhw;
		DWORD color;
	};
}