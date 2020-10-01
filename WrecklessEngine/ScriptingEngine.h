#pragma once
#include <string>
#include <memory>
#include <unordered_map>
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
		static void Initialize(const char* filename);
		static ScriptDomain& GetDomain();

		template<typename FuncPtr>
		static void DirectBindCallback(const std::string& fullName, FuncPtr funcPtr)
		{
			mono_add_internal_call(fullName.c_str(), (const void*)funcPtr);
		}
	private:
		static std::unique_ptr<ScriptDomain> m_Domain;
	};
}