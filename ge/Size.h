#pragma once

namespace ge {
	struct Size
	{
		Size() : width(0), height(0) {}
		Size(int width, int height) : width(width), height(height) {}
		int width, height;
	};
}
