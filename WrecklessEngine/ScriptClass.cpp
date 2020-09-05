#include "ScriptClass.h"
#include "ScriptingError.h"
#include "ScriptField.h"

namespace Scripting
{
    ScriptClass::ScriptClass(MonoClass* pClass, MonoDomain* pDomain)
        : m_pClass(pClass), m_pDomain(pDomain)
    {
        MonoMethod* m;
        MonoClassField* f;
        void* iter = nullptr;

        while ((m = mono_class_get_methods(m_pClass, &iter)) != nullptr)
            m_Methods[mono_method_get_name(m)] = std::make_shared<ScriptMethod>(m);

        iter = nullptr;

        while ((f = mono_class_get_fields(m_pClass, &iter)) != nullptr)
            m_Fields[mono_field_get_name(f)] = std::make_shared<ScriptField>(m_pClass, f);
    }
    ScriptObject Scripting::ScriptClass::CreateInstance()
    {
        MonoObject* object = mono_object_new(m_pDomain, m_pClass);

        mono_runtime_object_init(object);

        return ScriptObject(object, m_Fields, m_Methods);
    }
    void ScriptClass::Invoke(const std::string& method, ParameterList params)
    {
        auto iter = m_Methods.find(method);

        if (iter != m_Methods.end())
            iter->second->Invoke(params, nullptr);
        else
            SCRIPT_ERROR("Cannot find method");
    }
}
