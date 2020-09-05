#include "GamePadCSharp.h"
#include "ScriptingEngine.h"

namespace Scripting
{
    using namespace Input;

    void GamePadCSharp::Bind()
    {
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::IsPressed", IsPressed);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::LeftTrigger", LeftTrigger);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::RightTrigger", RightTrigger);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::SetVibration", SetVibration);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::GetVibration", GetVibration);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::LeftStick", LeftStick);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::RightStick", RightStick);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::SetSensitivity", SetSensitivity);
        ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.GamePad::GetSensitivity", GetSensitivity);
    }

    bool GamePadCSharp::IsPressed(Input::Button btn)
    {
        return GamePad::Get().IsPressed(btn);
    }
    float GamePadCSharp::LeftTrigger()
    {
        return GamePad::Get().LeftTrigger();
    }
    float GamePadCSharp::RightTrigger()
    {
        return GamePad::Get().RightTrigger();
    }
    void GamePadCSharp::SetVibration(int leftMotor, int rightMotor)
    {
        GamePad::Get().SetVibration(leftMotor, rightMotor);
    }
    Input::VibrationState GamePadCSharp::GetVibration()
    {
        return GamePad::Get().GetVibration();
    }
    Input::StickCoords GamePadCSharp::LeftStick()
    {
        return GamePad::Get().LeftStick();
    }
    Input::StickCoords GamePadCSharp::RightStick()
    {
        return GamePad::Get().RightStick();
    }

    void GamePadCSharp::SetSensitivity(int sensitivity)
    {
        GamePad::Get().SetSensitivity(sensitivity);
    }
    int GamePadCSharp::GetSensitivity()
    {
        return GamePad::Get().GetSensitivity();
    }
}
