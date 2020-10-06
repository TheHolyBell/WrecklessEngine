#include "ScriptingEngine.h"

static const char* assembly_name;

namespace Scripting
{
    std::unique_ptr<ScriptDomain> ScriptingEngine::m_Domain = nullptr;

    void ScriptingEngine::Initialize(const char* filename)
    {
        assembly_name = filename;
        mono_set_assemblies_path("D:\\VisualStudio\\C++\\WrecklessEngine\\Vendor\\Mono\\lib");
        m_Domain = std::make_unique<ScriptDomain>(filename);
    }
    ScriptDomain& ScriptingEngine::GetDomain()
    {
        return *m_Domain;
    }
}
