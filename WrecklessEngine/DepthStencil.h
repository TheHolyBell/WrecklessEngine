#pragma once
#include "Bindable.h"
#include "IDepthStencilState.h"

namespace Bindable
{
	class DepthStencil : public IBindable
	{
	public:
		enum class Func
		{
			NEVER = 1,
			LESS = 2,
			EQUAL = 3,
			LEQUAL = 4,
			GREATER = 5,
			NEQUAL = 6,
			GEQUAL = 7,
			ALWAYS = 8
		};
		enum class StencilMode
		{
			Off,
			Write,
			Mask
		};
	public:
		DepthStencil(Func depthFunc = Func::LESS, bool depthEnable = true, Func stencilFunc = Func::NEVER, StencilMode stencilMode = StencilMode::Off);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;

		static Ref<DepthStencil> Resolve(Func depthFunc = Func::LESS, bool depthEnable = true, Func stencilFunc = Func::NEVER, StencilMode stencilMode = StencilMode::Off);
		static std::string GenerateUID(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode);

	private:
		Ref<Graphics::IDepthStencilState> m_pDepthStencilState;
		Func m_DepthFunc;
		Func m_StencilFunc;
		StencilMode m_StencilMode;
		bool m_bDepth = false;
	};
}