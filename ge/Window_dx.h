#pragma once

#include "Window.h"
#include "Device_dx.h"

namespace ge {
	class Window_dx : public Window
	{
	public:
		//~Window_dx() override = default;

	private:
		Device_dx d;
	};
}
