#include "Gamepad.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace Input
{
	GamePad& GamePad::Get()
	{
		static GamePad _Instance;
		return _Instance;
	}

	bool GamePad::IsPressed(Button btn) const
	{
		if (StateChanged())
			return (m_State.Gamepad.wButtons & (int)btn) != 0;
		return false;
	}

	void GamePad::UpdateState()
	{
		
		XInputGetState(m_PlayerID, &m_State);
		if (m_PrevDWPacketNumber != m_State.dwPacketNumber)
		{
			m_PrevDWPacketNumber = m_State.dwPacketNumber;
			m_bStateChanged = true;
		}
		else
		{
			m_bStateChanged = false;
		}
	}

	float GamePad::LeftTrigger() const
	{
		return m_State.Gamepad.bLeftTrigger / 255.0f;
	}

	float GamePad::RightTrigger() const
	{
		return m_State.Gamepad.bRightTrigger / 255.0f;
	}

	void GamePad::SetVibration(int leftMotor, int rightMotor)
	{
		m_LeftMotorVibration = leftMotor;
		m_RightMotorVibration = rightMotor;

		XINPUT_VIBRATION _vibration = {};
		_vibration.wLeftMotorSpeed = short(leftMotor * 655);
		_vibration.wRightMotorSpeed = short(rightMotor * 655);
		XInputSetState(m_PlayerID, &_vibration);
	}

	VibrationState GamePad::GetVibration() const
	{
		return { m_LeftMotorVibration, m_RightMotorVibration };
	}

	StickCoords GamePad::LeftStick() const
	{
		float normX = std::max(-1.0f, (float)m_State.Gamepad.sThumbLX / 32767);
		float normY = std::max(-1.0f, (float)m_State.Gamepad.sThumbLY / 32767);

		float deadzoneX = 0.13f;
		float deadzoneY = 0.13f;

		normX = (abs(normX) < deadzoneX ? 0 : normX);
		normY = (abs(normY) < deadzoneY ? 0 : normY);

		return { normX, normY };
	}

	StickCoords GamePad::RightStick() const
	{
		float normX = std::max(-1.0f, (float)m_State.Gamepad.sThumbRX / 32767);
		float normY = std::max(-1.0f, (float)m_State.Gamepad.sThumbRY / 32767);
		float deadzoneX = 0.13f;
		float deadzoneY = 0.13f;

		normX = (abs(normX) < deadzoneX ? 0 : normX);
		normY = (abs(normY) < deadzoneY ? 0 : normY);

		return { normX, normY };
	}

	bool GamePad::StateChanged() const
	{
		return m_bStateChanged;
	}

	void GamePad::SetSensitivity(int sensitivity)
	{
		m_Sensitivity = sensitivity;
	}

	int GamePad::GetSensitivity() const
	{
		return m_Sensitivity;
	}

	GamePad::GamePad()
		: m_PlayerID(-1), m_Sensitivity(1500)
	{
		for (int i = 0; i < XUSER_MAX_COUNT && m_PlayerID == -1; ++i)
		{
			m_State = {};
			if (XInputGetState(i, &m_State) == ERROR_SUCCESS)
			{
				m_PlayerID = i;
				m_PrevDWPacketNumber = m_State.dwPacketNumber;
			}
		}

		assert(m_PlayerID != -1 && "Failed to connect Gamepad device");
	}
}