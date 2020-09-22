#pragma once

#include "CommonInclude.h"
#include "GraphicsFormats.h"

namespace Graphics
{
	enum class TEXTURE_ADDRESS_MODE
	{
		TEXTURE_ADDRESS_WRAP = 1,
		TEXTURE_ADDRESS_MIRROR = 2,
		TEXTURE_ADDRESS_CLAMP = 3,
		TEXTURE_ADDRESS_BORDER = 4,
		TEXTURE_ADDRESS_MIRROR_ONCE = 5
	};

	struct SAMPLER_DESC
	{
		SAMPLE_FILTER Filter;
		TEXTURE_ADDRESS_MODE AddressU;
		TEXTURE_ADDRESS_MODE AddressV;
		TEXTURE_ADDRESS_MODE AddressW;
		float MipLODBias;
		unsigned MaxAnisotropy;
		COMPARISON_FUNC ComparisonFunc;
		float BorderColor[4];
		float MinLOD;
		float MaxLOD;
	};

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