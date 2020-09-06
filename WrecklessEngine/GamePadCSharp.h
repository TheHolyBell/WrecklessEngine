#pragma once

#include "GamePad.h"
#include "Buttons.h"

namespace Scripting
{
	class GamePadCSharp
	{
	public:
		static void Bind();

		static bool IsPressed(Input::Button btn);

		static float LeftTrigger();

		static float RightTrigger();

		static void SetVibration(Input::VibrationState state);

		static Input::VibrationState GetVibration();

		static Input::StickCoords LeftStick();

		static Input::StickCoords RightStick();

		static void SetSensitivity(int sensitivity);

		static int GetSensitivity();
	};
}