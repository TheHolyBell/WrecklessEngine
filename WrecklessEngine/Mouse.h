#pragma once
#include <utility>

namespace Input
{
	class Mouse
	{
	public:
		static void SetPosition(int x, int y);
		static int GetPositionX();
		static int GetPositionY();
		static std::pair<int, int> GetPosition();

		static bool IsLeftMouseDown();
		static bool IsRightMouseDown();
	};
}