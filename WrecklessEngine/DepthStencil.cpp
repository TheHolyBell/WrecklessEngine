#include "DepthStencil.h"
#include "Renderer.h"
#include "BindableCodex.h"

#include <d3d11.h>

static D3D11_COMPARISON_FUNC GetD3D11ComparisonFunc(Bindable::DepthStencil::Func func)
{
	using Func = Bindable::DepthStencil::Func;

	switch (func)
	{
	case Func::NEVER:
		return D3D11_COMPARISON_NEVER;
	case Func::LESS:
		return D3D11_COMPARISON_LESS;
	case Func::EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case Func::LEQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	case Func::GREATER:
		return D3D11_COMPARISON_GREATER;
	case Func::NEQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case Func::GEQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case Func::ALWAYS:
		return D3D11_COMPARISON_ALWAYS;
	}
}

static const std::string ConvertFuncToString(Bindable::DepthStencil::Func func)
{
	using Func = Bindable::DepthStencil::Func;

	switch (func)
	{
	case Func::NEVER:
		return "FUNC_NEVER";
	case Func::LESS:
		return "FUNC_LESS";
	case Func::EQUAL:
		return "FUNC_EQUAL";
	case Func::LEQUAL:
		return "FUNC_LEQUAL";
	case Func::GREATER:
		return "FUNC_GREATER";
	case Func::NEQUAL:
		return "FUNC_NEQUAL";
	case Func::GEQUAL:
		return "FUNC_GEQUAL";
	case Func::ALWAYS:
		return "FUNC_ALWAYS";
	}
}

static const std::string ConvertModeToString(Bindable::DepthStencil::StencilMode mode)
{
	using Mode = Bindable::DepthStencil::StencilMode;

	switch (mode)
	{
	case Mode::Off:
		return "Off";
	case Mode::Mask:
		return "Mask";
	case Mode::Write:
		return "Write";
	}
}

namespace Bindable
{
	DepthStencil::DepthStencil(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
		: m_DepthFunc(depthFunc), m_StencilFunc(stencilFunc), m_StencilMode(stencilMode), m_bDepth(depthEnable)
	{
		D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

		desc.DepthFunc = GetD3D11ComparisonFunc(depthFunc);
		desc.FrontFace.StencilFunc = GetD3D11ComparisonFunc(stencilFunc);
		desc.DepthEnable = m_bDepth;

		if (stencilMode == StencilMode::Write)
		{
			desc.StencilEnable = TRUE;
			desc.StencilWriteMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		else if (stencilMode == StencilMode::Mask)
		{
			desc.DepthEnable = FALSE;
			desc.StencilEnable = TRUE;
			desc.StencilReadMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		}

		m_pDepthStencilState = Graphics::Renderer::GetDevice()->CreateDepthStencilState(*reinterpret_cast<Graphics::DEPTH_STENCIL_DESC*>(&desc));
	}

	void DepthStencil::Bind() noxnd
	{
		Graphics::Renderer::GetRenderContext()->BindDepthStencilState(m_pDepthStencilState);
	}

	std::string DepthStencil::GetUID() const noexcept
	{
		return GenerateUID(m_DepthFunc, m_bDepth, m_StencilFunc, m_StencilMode);
	}
	Ref<DepthStencil> DepthStencil::Resolve(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
	{
		return Codex::Resolve<DepthStencil>(depthFunc, depthEnable, stencilFunc, stencilMode);
	}
	std::string DepthStencil::GenerateUID(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
	{
		return ConvertFuncToString(depthFunc) + "#" + ConvertFuncToString(stencilFunc) + "#" + ConvertModeToString(stencilMode);
	}
}