#pragma once

#include "utility.h"

namespace ge {
	class Window;

	class Device
	{
	public:
		Device(Window* wnd) : wnd(wnd) {}
		virtual ~Device() = default;

		virtual void begin() {}
		virtual void end() {}

	private:
		friend class Device_dx;

		Window* wnd;
	};
}