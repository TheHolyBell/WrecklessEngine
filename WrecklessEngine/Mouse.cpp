#include "Mouse.h"
#include <Windows.h>

namespace Input
{

	void Mouse::SetPosition(int x, int y)
	{
		SetCursorPos(x, y);
	}

	int Mouse::GetPositionX()
	{
		return GetPosition().first;
	}

	int Mouse::GetPositionY()
	{
		return GetPosition().second;
	}

	std::pair<int, int> Mouse::GetPosition()
	{
		POINT pos;
		GetCursorPos(&pos);
		return { pos.x, pos.y };
	}

	bool Mouse::IsLeftMouseDown()
	{
		return GetKeyState(VK_LBUTTON);
	}

	bool Mouse::IsRightMouseDown()
	{
		return GetKeyState(VK_RBUTTON);
	}

}