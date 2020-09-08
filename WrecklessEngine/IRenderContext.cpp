#include "IRenderContext.h"

namespace Graphics
{
	void IRenderContext::SetOutputTargets(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil)
	{
		std::vector<Ref<IRenderTarget>> target{ render_target };
		SetOutputTargets(target, depth_stencil);
	}
}