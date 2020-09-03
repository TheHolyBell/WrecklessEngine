#pragma once
#include "Keys.h"
#include <dinput.h>
#include <wrl/client.h>

namespace Input
{
	class Keyboard
	{
	public:
		static void Initialize(HWND hwnd);

		static void Update();

		static bool CapsLock();
		static bool NumLock();

		static bool IsKeyDown(Keys key);
		static bool IsKeyUp(Keys key);
	private:
		static Microsoft::WRL::ComPtr<IDirectInputDevice8> m_pKeyboard;
		static Keys m_KeyboardState[256];
	};
}