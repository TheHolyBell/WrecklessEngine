#include "ScriptMethod.h"
#include "ScriptObject.h"
#include "CommonInclude.h"
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

		MonoObject* exception = nullptr;
		mono_runtime_invoke(m_pMethod, obj, params.GetArgs(), &exception);
		if (exception != nullptr)
		{
			Scripting::String str((MonoString*)mono_property_get_value(mono_class_get_property_from_name(mono_object_get_class(exception), "Message"), exception, nullptr, nullptr));
			SCRIPT_ERROR(str.ToUTF8());
		}
	}
}