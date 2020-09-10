#pragma once
#include "KeyCode.h"
#include <dinput.h>
#include <wrl/client.h>

namespace Graphics
{
	class Win32Window;
}

namespace Input
{
	class Keyboard
	{
		friend class Graphics::Win32Window;
	public:
		static bool CapsLock();
		static bool NumLock();

		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);
	private:
		static void KeyPressed(KeyCode key);
		static void KeyReleased(KeyCode key);
	private:
		static bool m_KeyboardState[256];
	};
}