#pragma once
#include "CommonInclude.h"

namespace Graphics
{
	enum class FILL_MODE
	{
		FILL_WIREFRAME = 2,
		FILL_SOLID = 3
	};

	enum class CULL_MODE
	{
		CULL_NONE = 1,
		CULL_FRONT = 2,
		CULL_BACK = 3
	};

	struct RASTERIZER_DESC
	{
		FILL_MODE FillMode;
		CULL_MODE CullMode;
		int FrontCounterClockwise;
		int DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		int DepthClipEnable;
		int ScissorEnable;
		int MultisampleEnable;
		int AntialiasedLineEnable;
	};

	class IRasterizer
	{
	public:
		IRasterizer() = default;

		IRasterizer(const IRasterizer& rhs) = delete;
		IRasterizer& operator=(const IRasterizer& rhs) = delete;

		IRasterizer(IRasterizer&& rhs) = delete;
		IRasterizer& operator=(IRasterizer&& rhs) = delete;

		virtual ~IRasterizer() = default;

		virtual void* GetNativePointer() const PURE;
	};
}