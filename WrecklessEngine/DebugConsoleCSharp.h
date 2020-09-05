#pragma once

#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

namespace Scripting
{
	class Debug
	{
	public:
		static void Bind();

		static void Log(MonoString* message);

		static void Assert(bool condition, MonoString* message);
	};
}