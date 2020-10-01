#include "ScriptObject.h"
#include "ScriptProperty.h"
#include "ScriptField.h"
#include "ScriptingError.h"
#include "Hasher.h"

namespace Scripting
{
    ScriptObject::ScriptObject(MonoObject* pObject,
        const std::unordered_map<size_t, std::shared_ptr<ScriptMethod>>& methods)
        : m_pObject(pObject), m_Methods(methods)
    {
    }
    void ScriptObject::Invoke(const std::string& method, ParameterList params)
    {
        auto iter = m_Methods.find(HASH(method));

        if (iter != m_Methods.end())
            iter->second->Invoke(params, this);
        else
            SCRIPT_ERROR("Cannot find method: " + method);
    }
    MonoObject* ScriptObject::GetObjectPointer() const
    {
        return m_pObject;
    }
    std::string ScriptObject::GetTypeName() const
    {
        //mono_type_get_name(mono_field_get_type(m_pField));
        return mono_type_get_name(mono_class_get_type(mono_object_get_class(m_pObject)));
    }
    ScriptProperty& ScriptObject::GetProperty(const std::string& name)
    {
        auto iter = m_Properties.find(HASH(name));
        if (iter != m_Properties.end())
            return *iter->second;
        else
        {
            MonoClass* pClass = mono_object_get_class(m_pObject);
            MonoProperty* pProperty = nullptr;
            pProperty = mono_class_get_property_from_name(pClass, name.c_str());
            if (pProperty == nullptr)
                SCRIPT_ERROR("Cannot find property: " + name);

            auto prop = std::make_shared<ScriptProperty>(m_pObject, pProperty);
            m_Properties[HASH(name)] = prop;
            return *prop;
        }
    }

    static FieldType GetWrecklessFieldType(MonoType* monoType)
    {
        int type = mono_type_get_type(monoType);
        switch (type)
        {
        case MONO_TYPE_R4: return FieldType::Float;
        case MONO_TYPE_I4: return FieldType::Int;
        case MONO_TYPE_U4: return FieldType::UnsignedInt;
        case MONO_TYPE_STRING: return FieldType::String;
        case MONO_TYPE_VALUETYPE:
        {
            char* name = mono_type_get_name(monoType);
            if (strcmp(name, "WrecklessScript.Core.Math.Vector2") == 0) return FieldType::Vec2;
            if (strcmp(name, "WrecklessScript.Core.Math.Vector3") == 0) return FieldType::Vec3;
            if (strcmp(name, "WrecklessScript.Core.Math.Vector4") == 0) return FieldType::Vec4;
        }
        }
        return FieldType::None;
    }

    ScriptField& ScriptObject::GetField(const std::string& name)
    {
        auto iter = m_Fields.find(HASH(name));

        if (iter != m_Fields.end())
            return *iter->second;
        else
        {
            MonoClass* pClass = mono_object_get_class(m_pObject);
            MonoClassField* pField = mono_class_get_field_from_name(pClass, name.c_str());
            

            if (pField == nullptr)
                SCRIPT_ERROR("Couldn't find field: " + name);

            MonoType* fieldType = mono_field_get_type(pField);
            FieldType wrecklessFieldType = GetWrecklessFieldType(fieldType);

            auto field = std::make_shared<ScriptField>(m_pObject, pField, wrecklessFieldType);
            m_Fields[HASH(name)] = field;
            return *field;
        }
    }
}