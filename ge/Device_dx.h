#pragma once

#include "utility_dx.h"

namespace ge {
	class Window;

	class Device_dx
	{
	public:
		Device_dx(Window* wnd);

		void updatePresentParameters();

	private:
		Window* wnd;

		static LPDirectx g_pD3D;
		static D3DDISPLAYMODE d3ddm;

		D3DPRESENT_PARAMETERS d3dpp;
		LPDevice g_pDevice;
	};
}
