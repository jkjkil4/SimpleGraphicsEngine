#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

#define GE_NAMESPACE using namespace ge; using namespace std
#define _T TEXT

#define WMU_CREATE_WND (WM_USER + 1)

typedef LPDIRECT3DTEXTURE9 LPTexture;
typedef LPDIRECT3DSURFACE9 LPSurface;
typedef LPD3DXSPRITE LPSprite;
typedef LPD3DXFONT LPFont;
typedef LPDIRECT3DVERTEXSHADER9 LPVertexShader;
typedef LPDIRECT3DPIXELSHADER9 LPPixelShader;
typedef LPD3DXCONSTANTTABLE LPConstantTable;

typedef LPDIRECT3D9 LPDirectx;
typedef LPDIRECT3DDEVICE9 LPDevice;

typedef D3DXIMAGE_INFO ImageInfo;

namespace ge {
	template<typename T>inline void SafeDelete(T*& t) {
		if (t) {
			delete t;
			t = nullptr;
		}
	}
	template<typename T>inline void SafeDeleteArray(T*& t) {
		if (t) {
			delete[] t;
			t = nullptr;
		}
	}

}