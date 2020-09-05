#include "ScriptDomain.h"
#include "ScriptingError.h"

namespace Scripting
{
	ScriptDomain::ScriptDomain(const char* filename)
	{
		m_pDomain = mono_jit_init(filename);

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
	ScriptClass ScriptDomain::GetClass(const std::string& name_space, const std::string& name)
	{
		MonoClass* pClass = mono_class_from_name(m_pImage, name_space.c_str(), name.c_str());
		if (pClass == nullptr)
			SCRIPT_ERROR("Failed to find class");
		return ScriptClass(pClass, m_pDomain);
	}
	ScriptDomain::~ScriptDomain()
	{
		mono_jit_cleanup(m_pDomain);
	}
}