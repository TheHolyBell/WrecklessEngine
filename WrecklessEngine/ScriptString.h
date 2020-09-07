#pragma once

#include <string>

#include <mono/jit/jit.h>

#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

#include "IParameter.h"

namespace Scripting
{
	class String : public IParameter
	{
	public:

		String(MonoString* str);
		String(const char* str);
		String(const std::string& str);

		String(const String& rhs);
		String& operator=(const String& rhs);

		String(String&& rhs);
		String& operator=(String&& rhs);

		~String();

		int Length() const;

		const char* ToUTF8();

		virtual void* ptr() noexcept override;
		MonoString* GetString() const;

	private:
		MonoString* m_pString = nullptr;
		char* m_pBuffer = nullptr;
	};
}