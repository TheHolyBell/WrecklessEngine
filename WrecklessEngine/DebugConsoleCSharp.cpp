#include "DebugConsoleCSharp.h"
#include "InputOutputStream.h"
#include "Manipulators.h"
#include "ScriptingEngine.h"

#include <cassert>

#include "ScriptString.h"

namespace Scripting
{
	void Debug::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Debug::Assert", Assert);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Debug::Log", Log);
	}
	void Debug::Log(MonoString* message)
	{
		IO::cout << String(message).ToUTF8() << IO::endl;
	}
	void Debug::Assert(bool condition, MonoString* message)
	{
		if (message != nullptr)
			assert(condition && String(message).ToUTF8());
		else
			assert(condition);
	}
}