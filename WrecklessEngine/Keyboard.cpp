#include "Keyboard.h"
#include <Windows.h>
#include "HrException.h"
#include "WindowsThrowMacros.h"

static LPDIRECTINPUT8 g_pDirectInput;

namespace Input
{
	bool Keyboard::m_KeyboardState[256] = {};

	bool Keyboard::CapsLock()
	{
		return m_KeyboardState[(int)KeyCode::CapsLock];
	}
	bool Keyboard::NumLock()
	{
		return m_KeyboardState[(int)KeyCode::NumLock];
	}
	bool Keyboard::IsKeyDown(KeyCode key)
	{
		return m_KeyboardState[(int)key];
	}
	bool Keyboard::IsKeyUp(KeyCode key)
	{
		return !IsKeyDown(key);
	}


	void Keyboard::KeyPressed(KeyCode key)
	{
		m_KeyboardState[(int)key] = true;
	}
	void Keyboard::KeyReleased(KeyCode key)
	{
		m_KeyboardState[(int)key] = false;
	}
}