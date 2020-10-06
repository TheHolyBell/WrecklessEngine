#include "ScriptDomain.h"
#include "ScriptingError.h"
#include "Hasher.h"

namespace Scripting
{
	ScriptDomain::ScriptDomain(const char* filename)
	{
		m_pDomain = mono_jit_init(filename);
		//m_pDomain = mono_jit_init_version("myapp", "v4.0.30319");
		//m_pDomain = mono_jit_init_version("myapp", "v2.0.50727");
		if (m_pDomain == nullptr)
			SCRIPT_ERROR("m_pDomain is null");

		m_pAssembly = mono_domain_assembly_open(m_pDomain, filename);

		if (m_pAssembly == nullptr)
			SCRIPT_ERROR("m_pAssembly is null");

		m_pImage = mono_assembly_get_image(m_pAssembly);

		if (m_pImage == nullptr)
			SCRIPT_ERROR("m_pImage is null");
	}
	void ScriptDomain::ExecuteMain(int argc, char** argv)
	{
		mono_jit_exec(m_pDomain, m_pAssembly, argc, argv);
	}
	ScriptClass& ScriptDomain::GetClass(const std::string& full_class_name)
	{
		size_t token = HASH(full_class_name);
		auto iter = m_Classes.find(token);
		size_t dotPos = full_class_name.find_last_of('.');
		
		if(dotPos == std::string::npos)
			SCRIPT_ERROR("Failed to find class");

		std::string name_space = full_class_name.substr(0, dotPos);
		std::string class_name = full_class_name.substr(dotPos + 1);

		if (iter != m_Classes.end())
			return *iter->second;


		else
		{
			MonoClass* pClass = mono_class_from_name(m_pImage, name_space.c_str(), class_name.c_str());
			if (pClass == nullptr)
				SCRIPT_ERROR("Failed to find class");
			
			auto klass = std::make_shared<ScriptClass>(pClass, m_pDomain);
			m_Classes[token] = klass;

			return *klass;
		}
	}
	bool ScriptDomain::IsClassExists(const std::string& full_class_name)
	{
		size_t dotPos = full_class_name.find_last_of('.');

		if (dotPos == std::string::npos)
			return false;

		std::string name_space = full_class_name.substr(0, dotPos);
		std::string class_name = full_class_name.substr(dotPos + 1);

		MonoClass* pClass = mono_class_from_name(m_pImage, name_space.c_str(), class_name.c_str());

		return pClass != nullptr;
	}
	ScriptDomain::~ScriptDomain()
	{
		mono_jit_cleanup(m_pDomain);
	}
}