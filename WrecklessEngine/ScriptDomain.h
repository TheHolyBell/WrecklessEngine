#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptClass.h"

namespace Scripting
{
	class ScriptDomain
	{
	public:
		ScriptDomain() = default;
		ScriptDomain(const char* filename);
		// @ Optional since .DLL files doesn't have that method
		void ExecuteMain(int argc, char** argv);

		ScriptClass& GetClass(const std::string& full_class_name);
		bool IsClassExists(const std::string& full_class_name);

		~ScriptDomain();
	private:
		MonoDomain* m_pDomain = nullptr;
		MonoAssembly* m_pAssembly = nullptr;
		MonoImage* m_pImage = nullptr;
		std::unordered_map<size_t, std::shared_ptr<ScriptClass>> m_Classes;
	};
}