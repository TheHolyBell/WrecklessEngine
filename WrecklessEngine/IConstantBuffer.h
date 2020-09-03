#pragma once

#include "CommonInclude.h"
#include "DynamicConstant.h"

namespace Graphics
{
	class IConstantBuffer
	{
	public:
		
		virtual Dynamic::Buffer& GetBuffer() PURE;

		virtual ~IConstantBuffer() = default;
	};
}