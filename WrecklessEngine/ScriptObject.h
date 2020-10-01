#pragma once
#include "CommonInclude.h"
#include <string>
#include <unordered_map>
#include <memory>

#include "ScriptMethod.h"

#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptProperty.h"

namespace Scripting
{
	class ScriptMethod;
	class ScriptField;
	class ScriptProperty;

	class ScriptObject
	{
	public:
		ScriptObject(MonoObject* pObject,
			const std::unordered_map<size_t, std::shared_ptr<ScriptMethod>>& methods);

		void Invoke(const std::string& method, ParameterList params = {});

		MonoObject* GetObjectPointer() const;

		std::string GetTypeName() const;

		ScriptProperty& GetProperty(const std::string& name);
		ScriptField& GetField(const std::string& name);
	private:
		MonoObject* m_pObject;
		std::unordered_map<size_t, Ref<ScriptField>> m_Fields;
		std::unordered_map<size_t, Ref<ScriptMethod>> m_Methods;
		std::unordered_map<size_t, Ref<ScriptProperty>> m_Properties;
	};
}