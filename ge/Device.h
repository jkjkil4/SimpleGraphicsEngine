#pragma once

#include "utility.h"
#include "Object.h"

namespace ge {
	class Window;

	class Device : public Object
	{
	public:
		Device(Window* wnd) : wnd(wnd) {}
		~Device() override = default;

		virtual void begin() = 0;
		virtual void end() = 0;

	private:
		friend class Device_dx;

		Window* wnd;
	};
}