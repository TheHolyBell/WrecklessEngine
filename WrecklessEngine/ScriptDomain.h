#pragma once

#include <string>
#include <vector>

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
		ScriptDomain(const char* filename);
		// @ Optional since .DLL files doesn't have that method
		void ExecuteMain(int argc, char** argv);

		ScriptClass GetClass(const std::string& name_space, const std::string& name);

		~ScriptDomain();
	private:
		MonoDomain* m_pDomain;
		MonoAssembly* m_pAssembly;
		MonoImage* m_pImage;
	};
}