#pragma once
#include <string>
#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptString.h"
#include "ScriptingError.h"

namespace Scripting
{
	class ScriptProperty
	{
	public:
		ScriptProperty(MonoObject* pObject, MonoProperty* pProperty);

		std::string GetName() const;

		bool HasSetProperty() const;
		bool HasGetProperty() const;

		template<typename T>
		T Get() const
		{
			if (m_pGetMethod == nullptr)
				SCRIPT_ERROR("Get method doesn't exists");

			MonoObject* result = mono_runtime_invoke(m_pGetMethod, m_pObject, nullptr, nullptr);

			return *(T*)mono_object_unbox(result);
		}
		
		template<typename T>
		void Set(T value)
		{
			if (m_pSetMethod == nullptr)
				SCRIPT_ERROR("Set method doesn't exists");

			void* params[1];
			params[0] = &value;

			mono_property_set_value(m_pProperty, m_pObject, params, nullptr);
		}

		template<>
		String Get() const
		{
			if (m_pGetMethod == nullptr)
				SCRIPT_ERROR("Get method doesn't exists");

			String str((MonoString*)mono_property_get_value(m_pProperty, m_pObject, nullptr, nullptr));
			return std::move(str);
		}


		template<>
		void Set(String value)
		{
			if (m_pSetMethod == nullptr)
				SCRIPT_ERROR("Set method doesn't exists");

			void* params[1];
			params[0] = value.ptr();

			mono_property_set_value(m_pProperty, m_pObject, params, nullptr);
		}

		template<>
		void Set(const char* str)
		{
			Set(String(str));
		}

	private:
		MonoProperty* m_pProperty;
		MonoMethod* m_pGetMethod;
		MonoMethod* m_pSetMethod;
		MonoObject* m_pObject;
	};
}