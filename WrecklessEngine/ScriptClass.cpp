#include "ScriptClass.h"
#include "ScriptingError.h"
#include "ScriptField.h"
#include "Hasher.h"

#include <mono/metadata/attrdefs.h>

namespace Scripting
{
    ScriptClass::ScriptClass(MonoClass* pClass, MonoDomain* pDomain)
        : m_pClass(pClass), m_pDomain(pDomain)
    {
        MonoMethod* m;
        MonoClassField* f;
        void* iter = nullptr;

        while ((m = mono_class_get_methods(m_pClass, &iter)) != nullptr)
            m_Methods[HASH(mono_method_get_name(m))] = std::make_shared<ScriptMethod>(m);
    }
    ScriptObject Scripting::ScriptClass::CreateInstance()
    {
        MonoObject* object = mono_object_new(m_pDomain, m_pClass);

        mono_runtime_object_init(object);

        return ScriptObject(object, m_Methods);
    }
    void ScriptClass::Invoke(const std::string& method, ParameterList params)
    {
        auto iter = m_Methods.find(HASH(method));

        if (iter != m_Methods.end())
            iter->second->Invoke(params, nullptr);
        else
            SCRIPT_ERROR("Cannot find method: " + method);
    }

    std::string ScriptClass::GetName() const
    {
        return mono_class_get_name(m_pClass);
    }

    std::vector<std::string> ScriptClass::GetFieldNames() const
    {
        std::vector<std::string> _Fields;

        MonoClassField* field;
        void* iter = nullptr;

        while ((field = mono_class_get_fields(m_pClass, &iter)) != nullptr)
        {
            if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_PUBLIC) == 0)
                continue;
            _Fields.push_back(mono_field_get_name(field));
        }

        

        return _Fields;
    }
    std::vector<std::string> ScriptClass::GetMethodNames() const
    {
        std::vector<std::string> _Methods;

        MonoMethod* method;
        void* iter = nullptr;

        while ((method = mono_class_get_methods(m_pClass, &iter)) != nullptr)
            _Methods.push_back(mono_method_get_name(method));

        return _Methods;
    }
    std::vector<std::string> ScriptClass::GetPropertyNames() const
    {
        std::vector<std::string> _Properties;

        MonoProperty* property;
        void* iter = nullptr;

        while ((property = mono_class_get_properties(m_pClass, &iter)) != nullptr)
            _Properties.push_back(mono_property_get_name(property));

        return _Properties;
    }
    std::vector<std::string> ScriptClass::GetEventNames() const
    {
        std::vector<std::string> _Events;

        MonoEvent* event;
        void* iter = nullptr;

        while ((event = mono_class_get_events(m_pClass, &iter)) != nullptr)
            _Events.push_back(mono_event_get_name(event));

        return _Events;
    }
}
