#pragma once
#include <string>
#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

namespace Scripting
{
	class ScriptField
	{
	public:
		ScriptField(MonoClass* pClass, MonoClassField* pField);
		std::string GetName() const;
		const char* GetType();

	private:
		MonoClassField* m_pField;
		MonoClass* m_pClass;
	};
}