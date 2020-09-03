#pragma once
#include "DynamicConstant.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace Dynamic
{
	
	class LayoutCodex
	{
	public:
		static CookedLayout Resolve(RawLayout&& layout) noxnd;
	private:
		static LayoutCodex& Get_() noexcept;
		std::unordered_map<std::string, std::shared_ptr<LayoutElement>> m_Map;
	};
}