#include "KeyboardCSharp.h"
#include "ScriptingEngine.h"
#include "Keyboard.h"

namespace Scripting
{
	void KeyboardCSharp::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Keyboard::CapsLock", Input::Keyboard::CapsLock);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Keyboard::IsKeyDown", Input::Keyboard::IsKeyDown);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Keyboard::IsKeyUp", Input::Keyboard::IsKeyUp);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Keyboard::NumLock", Input::Keyboard::NumLock);
	}
}