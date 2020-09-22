#pragma once
#include "CommonInclude.h"
#include "GraphicsFormats.h"

#include <d3d11.h>

namespace Graphics
{
	enum class STENCIL_OP
	{
		STENCIL_OP_KEEP = 1,
		STENCIL_OP_ZERO = 2,
		STENCIL_OP_REPLACE = 3,
		STENCIL_OP_INCR_SAT = 4,
		STENCIL_OP_DECR_SAT = 5,
		STENCIL_OP_INVERT = 6,
		STENCIL_OP_INCR = 7,
		STENCIL_OP_DECR = 8
	};

	struct DEPTH_STENCILOP_DESC
	{
		STENCIL_OP StencilFailOp;
		STENCIL_OP StencilDepthFailOp;
		STENCIL_OP StencilPassOp;
		COMPARISON_FUNC StencilFunc;
	};

	enum class DEPTH_WRITE_MASK
	{
		DEPTH_WRITE_MASK_ZERO = 0,
		DEPTH_WRITE_MASK_ALL = 1
	};

	struct DEPTH_STENCIL_DESC
	{
		int DepthEnable;
		DEPTH_WRITE_MASK DepthWriteMask;
		COMPARISON_FUNC DepthFunc;
		int StencilEnable;
		unsigned char StencilReadMask;
		unsigned char StencilWriteMask;
		DEPTH_STENCILOP_DESC FrontFace;
		DEPTH_STENCILOP_DESC BackFace;
	};

	class IDepthStencilState
	{
	public:
		IDepthStencilState() = default;

		IDepthStencilState(const IDepthStencilState& rhs) = delete;
		IDepthStencilState& operator=(const IDepthStencilState& rhs) = delete;

		IDepthStencilState(IDepthStencilState&& rhs) = delete;
		IDepthStencilState& operator=(IDepthStencilState&& rhs) = delete;

		virtual void* GetNativePointer() const PURE;

		virtual ~IDepthStencilState() = default;
	};
}