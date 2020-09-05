#include "MouseCSharp.h"
#include "ScriptingEngine.h"
#include "Mouse.h"

/*[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern void SetPosition(int x, int y);
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern int GetPositionX();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern int GetPositionY();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern MouseCoords GetPosition();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern bool IsLeftMouseDown();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern bool IsRightMouseDown();*/

namespace Scripting
{
	void MouseCSharp::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::GetPositionX", Input::Mouse::GetPositionX);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::GetPositionY", Input::Mouse::GetPositionY);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::SetPosition", Input::Mouse::SetPosition);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::GetPosition", Input::Mouse::GetPosition);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::IsLeftMouseDown", Input::Mouse::IsLeftMouseDown);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Input.Mouse::IsRightMouseDown", Input::Mouse::IsRightMouseDown);
	}
}