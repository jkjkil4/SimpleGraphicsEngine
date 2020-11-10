#pragma once

#include "Size.h"

namespace ge {
	class Window;

	struct Event
	{
		enum class Type { Resize };
	};

	typedef void(Window::* EventFunc)(Event*);
	struct SendEvent { Window* wnd; EventFunc func; Event* ev; bool single; };
	inline bool operator==(const SendEvent& se1, const SendEvent& se2) {
		return se1.wnd == se2.wnd && se1.func == se2.func;
	}

	struct ResizeEvent : public Event
	{
		ResizeEvent(const Size& size) : size(size) {}
		Size size;
	};
}