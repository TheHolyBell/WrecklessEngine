#pragma once

#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "CommonInclude.h"

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
	
		std::string GetName() const;

		// Convenience methods for runtime reflection of classes
		std::vector<std::string> GetFieldNames() const;
		std::vector<std::string> GetMethodNames() const;
		std::vector<std::string> GetPropertyNames() const;
		std::vector<std::string> GetEventNames() const;
	private:
		MonoClass* m_pClass;
		MonoDomain* m_pDomain;
		std::unordered_map<size_t, Ref<ScriptMethod>> m_Methods;
	};
}