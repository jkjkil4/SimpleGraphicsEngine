#pragma once

namespace ge {
	struct Point
	{
		Point(int x, int y) : x(x), y(y) {}
		int x, y;
	};

	struct PointF
	{
		PointF(Point pos) : x((float)pos.x), y((float)pos.y) {}
		PointF(float x, float y) : x(x), y(y) {}
		float x, y;
	};
}