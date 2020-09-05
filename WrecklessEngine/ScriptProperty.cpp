#include "ScriptProperty.h"

namespace Scripting
{
	ScriptProperty::ScriptProperty(MonoObject* pObject, MonoProperty* pProperty)
		: m_pProperty(pProperty), m_pObject(pObject)
	{
		m_pGetMethod = mono_property_get_get_method(m_pProperty);
		m_pSetMethod = mono_property_get_set_method(m_pProperty);
	}
	std::string ScriptProperty::GetName() const
	{
		return  mono_property_get_name(m_pProperty);
	}
	bool ScriptProperty::HasSetProperty() const
	{
		return m_pSetMethod != nullptr;
	}
	bool ScriptProperty::HasGetProperty() const
	{
		return m_pGetMethod != nullptr;
	}
}