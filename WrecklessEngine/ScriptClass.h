#pragma once

#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptObject.h"

namespace Scripting
{
	class ScriptProperty;
	class ScriptField;

	class ScriptClass
	{
	public:
		ScriptClass(MonoClass* pClass, MonoDomain* pDomain);

		ScriptObject CreateInstance();

		void Invoke(const std::string& method, ParameterList params = {});
	private:
		MonoClass* m_pClass;
		MonoDomain* m_pDomain;
		std::unordered_map<std::string, std::shared_ptr<ScriptField>> m_Fields;
		std::unordered_map<std::string, std::shared_ptr<ScriptMethod>> m_Methods;
	};
}