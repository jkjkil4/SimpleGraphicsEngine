#pragma once

#include "utility.h"
#include "Console.h"
#include "vf.h"
#include <string>

namespace ge {
	class Window
	{
	public:
		~Window();

		VAR_GET_FUNC(DefWidth, defWidth, int)
		VAR_GET_FUNC(DefHeight, defHeight, int)
		VAR_GET_FUNC(ViewWidth, viewWidth, int)
		VAR_GET_FUNC(ViewHeight, viewHeight, int)

	private:
		friend class SimpleGraphicsEngine;
		Window();

		LRESULT CALLBACK ProcWndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND g_hWnd = nullptr;
		std::wstring className;
		int defWidth, defHeight, viewWidth, viewHeight;
	};
}
