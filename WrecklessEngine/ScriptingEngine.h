#pragma once
#include <string>
#include "ScriptDomain.h"

namespace Scripting
{
	struct DirectoriesInfo
	{
		std::string lib_location;
		std::string etc_location;
	};

	class ScriptingEngine
	{
	public:
		static void Initialize(DirectoriesInfo* dirsInfo = nullptr);
		static ScriptDomain GetDomain(const char* filename);

		template<typename FuncPtr>
		static void DirectBindCallback(const std::string& fullName, FuncPtr funcPtr)
		{
			mono_add_internal_call(fullName.c_str(), (const void*)funcPtr);
		}
	};
}