#pragma once

namespace Input
{
	struct MouseCoords
	{
		int X;
		int Y;
	};

	class Mouse
	{
	public:
		static void SetPosition(int x, int y);
		static int GetPositionX();
		static int GetPositionY();
		static MouseCoords GetPosition();

		static bool IsLeftMouseDown();
		static bool IsRightMouseDown();
		static bool IsMiddleMouseDown();
	};
}