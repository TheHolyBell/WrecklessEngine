#include "Gamepad.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace Input
{
	Gamepad& Gamepad::Get()
	{
		static Gamepad _Instance;
		return _Instance;
	}

	bool Gamepad::IsPressed(Button btn) const
	{
		if (StateChanged())
			return (m_State.Gamepad.wButtons & (int)btn) != 0;
		return false;
	}

	void Gamepad::UpdateState()
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

	float Gamepad::LeftTrigger() const
	{
		return m_State.Gamepad.bLeftTrigger / 255.0f;
	}

	float Gamepad::RightTrigger() const
	{
		return m_State.Gamepad.bRightTrigger / 255.0f;
	}

	void Gamepad::SetVibration(int leftMotor, int rightMotor)
	{
		m_LeftMotorVibration = leftMotor;
		m_RightMotorVibration = rightMotor;

		XINPUT_VIBRATION _vibration = {};
		_vibration.wLeftMotorSpeed = short(leftMotor * 655);
		_vibration.wRightMotorSpeed = short(rightMotor * 655);
		XInputSetState(m_PlayerID, &_vibration);
	}

	std::pair<int, int> Gamepad::GetVibration() const
	{
		return { m_LeftMotorVibration, m_RightMotorVibration };
	}

	std::pair<float, float> Gamepad::LeftStick() const
	{
		float normX = std::max(-1.0f, (float)m_State.Gamepad.sThumbLX / 32767);
		float normY = std::max(-1.0f, (float)m_State.Gamepad.sThumbLY / 32767);

		float deadzoneX = 0.13f;
		float deadzoneY = 0.13f;

		normX = (abs(normX) < deadzoneX ? 0 : normX);
		normY = (abs(normY) < deadzoneY ? 0 : normY);

		return { normX, normY };
	}

	std::pair<float, float> Gamepad::RightStick() const
	{
		float normX = std::max(-1.0f, (float)m_State.Gamepad.sThumbRX / 32767);
		float normY = std::max(-1.0f, (float)m_State.Gamepad.sThumbRY / 32767);
		float deadzoneX = 0.13f;
		float deadzoneY = 0.13f;

		normX = (abs(normX) < deadzoneX ? 0 : normX);
		normY = (abs(normY) < deadzoneY ? 0 : normY);

		return { normX, normY };
	}

	bool Gamepad::StateChanged() const
	{
		return m_bStateChanged;
	}

	void Gamepad::SetSensitivity(int sensitivity)
	{
		m_Sensitivity = sensitivity;
	}

	int Gamepad::GetSensitivity() const
	{
		return m_Sensitivity;
	}

	Gamepad::Gamepad()
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