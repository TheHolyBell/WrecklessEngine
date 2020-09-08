#pragma once
#include <string>

namespace Misc
{
	class StringHelper
	{
	public:
		static std::wstring ToWide(const std::string& str);
		static std::string ToNarrow(const std::wstring& str);
	};
}