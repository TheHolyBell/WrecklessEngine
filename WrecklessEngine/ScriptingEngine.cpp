#include "ScriptingEngine.h"

namespace Scripting
{
    void ScriptingEngine::Initialize(DirectoriesInfo* dirsInfo)
    {
        if (dirsInfo != nullptr)
            mono_set_dirs(dirsInfo->lib_location.c_str(),
                dirsInfo->etc_location.c_str());
    }
    ScriptDomain ScriptingEngine::GetDomain(const char* filename)
    {
        return ScriptDomain(filename);
    }
}
