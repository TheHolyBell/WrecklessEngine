#include "StringHelper.h"

namespace Misc
{
	std::wstring StringHelper::ToWide(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}
	std::string StringHelper::ToNarrow(const std::wstring& str)
	{
		return std::string(str.begin(), str.end());
	}
}