#include "IRenderContext.h"

namespace Graphics
{
	void IRenderContext::SetOutputTargets(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil)
	{
		std::vector<Ref<IRenderTarget>> target{ render_target };
		SetOutputTargets(std::move(target), depth_stencil);
	}
}