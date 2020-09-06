#pragma once
#include "KeyCode.h"
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

		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);
	private:
		static Microsoft::WRL::ComPtr<IDirectInputDevice8> m_pKeyboard;
		static KeyCode m_KeyboardState[256];
	};
}