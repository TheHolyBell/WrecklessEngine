#include "Keyboard.h"
#include <Windows.h>
#include "HrException.h"
#include "WindowsThrowMacros.h"

static LPDIRECTINPUT8 g_pDirectInput;

namespace Input
{
	Microsoft::WRL::ComPtr<IDirectInputDevice8> Keyboard::m_pKeyboard;
	KeyCode Keyboard::m_KeyboardState[256] = {};

	void Keyboard::Initialize(HWND hwnd)
	{
		HRESULT hr;
		WRECK_HR(DirectInput8Create(GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&g_pDirectInput,
			nullptr));
		
		WRECK_HR(g_pDirectInput->CreateDevice(GUID_SysKeyboard,
			&m_pKeyboard, nullptr));

		WRECK_HR(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard));
		WRECK_HR(m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	}
	void Keyboard::Update()
	{
		m_pKeyboard->Acquire();

		m_pKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)(&m_KeyboardState));
	}
	bool Keyboard::CapsLock()
	{
		return (int)m_KeyboardState[(int)KeyCode::CapsLock] & 0x80;
	}
	bool Keyboard::NumLock()
	{
		return (int)m_KeyboardState[(int)KeyCode::NUMLOCK] & 0x80;
	}
	bool Keyboard::IsKeyDown(KeyCode key)
	{
		return (int)m_KeyboardState[(int)key] & 0x80;
	}
	bool Keyboard::IsKeyUp(KeyCode key)
	{
		return !IsKeyDown(key);
	}
}