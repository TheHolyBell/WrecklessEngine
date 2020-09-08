#include "ScriptingEngine.h"

namespace Scripting
{
    void ScriptingEngine::Initialize()
    {
        /*if (dirsInfo != nullptr)
            mono_set_dirs(dirsInfo->lib_location.c_str(),
                dirsInfo->etc_location.c_str());*/
        mono_set_assemblies_path("D:\\VisualStudio\\C++\\WrecklessEngine\\Vendor\\Mono\\lib");
    }
    ScriptDomain ScriptingEngine::GetDomain(const char* filename)
    {
        return ScriptDomain(filename);
    }
}
