#include "ScriptMethod.h"
#include "ScriptObject.h"

namespace Scripting
{
	ScriptMethod::ScriptMethod(MonoMethod* pMethod)
		: m_pMethod(pMethod)
	{
	}
	std::string ScriptMethod::GetName() const
	{
		return std::string(mono_class_get_name(mono_method_get_class(m_pMethod))) + "::" + mono_method_get_name(m_pMethod);
	}
	void ScriptMethod::Invoke(ParameterList params, ScriptObject* object)
	{
		MonoObject* obj = nullptr;

		if (object != nullptr)
			obj = object->GetObjectPointer();

		mono_runtime_invoke(m_pMethod, obj, params.GetArgs(), nullptr);
	}
}