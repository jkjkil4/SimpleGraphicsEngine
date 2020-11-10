#pragma once

#include <windef.h>

namespace ge {
	struct Rect
	{
		Rect() : x(0), y(0), width(0), height(0) {}
		Rect(RECT rect) : x(rect.left), y(rect.top), width(rect.right - rect.left), height(rect.bottom - rect.top) {}
		Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
		int x, y, width, height;
	};
}