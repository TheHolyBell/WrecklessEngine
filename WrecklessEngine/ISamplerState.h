#pragma once

#include "CommonInclude.h"

namespace Graphics
{
	class ISamplerState
	{
	public:
		ISamplerState() = default;

		ISamplerState(const ISamplerState& rhs) = delete;
		ISamplerState& operator=(const ISamplerState& rhs) = delete;

		ISamplerState(ISamplerState&& rhs) = delete;
		ISamplerState& operator=(ISamplerState&& rhs) = delete;

		virtual void* GetNativePointer() PURE;

		virtual ~ISamplerState() = default;
	};
}