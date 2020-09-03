#pragma once
#include <vector>
#include <string>
#include "CommonInclude.h"

namespace Graphics
{
	class InfoManager
	{
	public:

		virtual void Set() noexcept PURE;
		virtual std::vector<std::string> GetMessages() const PURE;

		virtual ~InfoManager() = default;
	};
}