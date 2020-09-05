#include "ScriptObject.h"
#include "ScriptProperty.h"
#include "ScriptField.h"
#include "ScriptingError.h"

namespace Scripting
{
    ScriptObject::ScriptObject(MonoObject* pObject, 
        const std::unordered_map<std::string, std::shared_ptr<ScriptField>>& fields, 
        const std::unordered_map<std::string, std::shared_ptr<ScriptMethod>>& methods)
        : m_pObject(pObject), m_Fields(fields), m_Methods(methods)
    {
    }
    void ScriptObject::Invoke(const std::string& method, ParameterList params)
    {
        auto iter = m_Methods.find(method);

        if (iter != m_Methods.end())
            iter->second->Invoke(params, this);
        else
            SCRIPT_ERROR("Cannot find method");
    }
    MonoObject* ScriptObject::GetObject() const
    {
        return m_pObject;
    }
    ScriptProperty ScriptObject::GetProperty(const std::string& name)
    {
        /*MonoClass* pClass = mono_object_get_class(m_pObject);
        MonoProperty* pProperty = mono_class_get_property_from_name(pClass, name.c_str());

        return ScriptProperty(m_pObject, pProperty);*/

        MonoClass* pClass = mono_object_get_class(m_pObject);
        auto iter = m_Properties.find(name);
        MonoProperty* pProperty = nullptr;
        if (iter != m_Properties.end())
            return *iter->second;
        else
        {
            void* it = nullptr;
            pProperty = mono_class_get_property_from_name(pClass, name.c_str());
            if(pProperty == nullptr)
                SCRIPT_ERROR("Cannot find property");

            auto prop = std::make_shared<ScriptProperty>(m_pObject, pProperty);
            m_Properties[name] = prop;
            return *prop;
        }
    }
    ScriptField ScriptObject::GetField(const std::string& name)
    {
        auto iter = m_Fields.find(name);

        if (iter != m_Fields.end())
            return *iter->second;

        SCRIPT_ERROR("Cannot find field");
    }
}