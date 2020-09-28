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
		return GetPosition().X;
	}

	int Mouse::GetPositionY()
	{
		return GetPosition().Y;
	}

	MouseCoords Mouse::GetPosition()
	{
		POINT pos;
		GetCursorPos(&pos);
		return { pos.x, pos.y };
	}

	bool Mouse::IsLeftMouseDown()
	{
		return (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
	}

	bool Mouse::IsRightMouseDown()
	{
		return (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
	}

	bool Mouse::IsMiddleMouseDown()
	{
		return (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
	}

}