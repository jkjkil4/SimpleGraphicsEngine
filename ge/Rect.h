#pragma once

namespace ge {
	struct Rect
	{
		Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
		int x, y, width, height;
	};
}