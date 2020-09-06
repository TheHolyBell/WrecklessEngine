#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <utility>
#include "Buttons.h"

namespace Input
{
	struct StickCoords
	{
		float X;
		float Y;
	};

	struct VibrationState
	{
		int LeftMotor;
		int RightMotor;
	};

	class GamePad
	{
	public:
		// @ Get instance of Gamepad class (singleton implementation)
		static GamePad& Get();

		// @ Checks whether particular button is pressed
		bool IsPressed(Button btn) const;
		// @ Updates state of XInput
		void UpdateState();

		// @ Returns normalized value of left trigger's pressure
		float LeftTrigger() const;
		// @ Returns normalized value of right trigger's pressure
		float RightTrigger() const;

		// @ Sets percentage of vibration power for both motors
		void SetVibration(VibrationState state);
		// @ Returns percentage of vibration power for both motors
		VibrationState GetVibration() const;

		// @ Returns normalized values of left stick's x- and y-axis tilt
		StickCoords LeftStick() const;
		// @ Returns normalized values of right stick's x- and y-axis tilt
		StickCoords RightStick() const;

		// @ Check whether state of Gamepad has been changed since last UpdateState()
		bool StateChanged() const;

		void SetSensitivity(int sensitivity);
		int GetSensitivity() const;
	private:
		GamePad();
		XINPUT_STATE m_State = {};
		int m_PlayerID;
		DWORD m_PrevDWPacketNumber;
		int m_LeftMotorVibration = 0;
		int m_RightMotorVibration = 0;
		int m_Sensitivity = 0;
		bool m_bStateChanged = false;
	};
}