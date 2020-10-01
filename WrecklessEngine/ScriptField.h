#pragma once
#include <string>
#include <mono/jit/jit.h>

#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "ScriptString.h"

namespace Scripting
{
	enum class FieldType
	{
		None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
	};

	class ScriptField
	{
	public:
		ScriptField(MonoObject* pObject, MonoClassField* pField, FieldType type);
		std::string GetName() const;
		FieldType GetType() const;

		template<typename T>
		void SetValue(T value)
		{
			mono_field_set_value(m_pObject, m_pField, &value);
		}

		template<typename T>
		T GetValue() const
		{
			T value;
			mono_field_get_value(m_pObject, m_pField, &value);
			return value;
		}

		template<>
		void SetValue(const char* str)
		{
			mono_field_set_value(m_pObject, m_pField, String(str).GetString());
		}

		template<>
		void SetValue(String str)
		{
			mono_field_set_value(m_pObject, m_pField, str.GetString());
		}

		template<>
		String GetValue()const
		{
			MonoString* str;
			mono_field_get_value(m_pObject, m_pField, &str);
			return String(str);
		}

	private:
		MonoClassField* m_pField = nullptr;
		MonoObject* m_pObject = nullptr;
		FieldType m_Type = FieldType::None;
	};
}