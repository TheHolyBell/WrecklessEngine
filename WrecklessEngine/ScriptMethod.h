#pragma once
#include <string>
#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptParameterList.h"

namespace Scripting
{
	class ScriptObject;

	class ScriptMethod
	{
	public:
		ScriptMethod(MonoMethod* pMethod);
		std::string GetName() const;
		void Invoke(ParameterList params = {}, ScriptObject* object = nullptr);

	private:
		MonoMethod* m_pMethod;
	};
}