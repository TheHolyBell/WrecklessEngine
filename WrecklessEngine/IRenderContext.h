#pragma once
#include "CommonInclude.h"

namespace Graphics
{
	class IRenderContext
	{
	public:
		virtual void* GetContextPointer() const PURE;

		virtual ~IRenderContext() = default;
	};
}