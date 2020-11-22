#pragma once

#include "utility_dx.h"
#include "Size.h"
#include "Object.h"

namespace ge {
	class Device_dx;

	class Image_dx : public Object
	{
	public:
		//Static -> usage = 0,						pool = D3DPOOL_MANAGED
		//Target -> usage = D3DUSAGE_RENDERTARGET,	pool = D3DPOOL_DEFAULT
		enum Type : bool { Static, Target };
		static inline DWORD GetUsage(Type type) { return type == Static ? 0 : D3DUSAGE_RENDERTARGET; }
		static inline D3DPOOL GetPool(Type type) { return type == Static ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT; }

		Image_dx(Device_dx& d, UINT w, UINT h, HRESULT* hr = nullptr);
		Image_dx(Device_dx& d, const std::wstring& path, UINT w = D3DX_FROM_FILE, UINT h = D3DX_FROM_FILE, Type type = Static, HRESULT* hr = nullptr);
		Image_dx(Device_dx& d, HMODULE hSrcModule, LPCWSTR pSrcResource, UINT w = D3DX_FROM_FILE, UINT h = D3DX_FROM_FILE, Type type = Static, HRESULT* hr = nullptr);
		~Image_dx() override;

		VAR_GET_FUNC(Size, size, Size)
		VAR_GET_FUNC(Width, size.width, int)
		VAR_GET_FUNC(Height, size.height, int)

	private:
		friend class Device_dx;

		Device_dx* d;
		Type type;

		void checkType();

		void onReleaseDevice();
		void onResetDevice();

		LPTexture g_pTexture;
		Size size;
	};
}
