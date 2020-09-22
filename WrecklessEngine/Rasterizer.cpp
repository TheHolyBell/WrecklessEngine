#include <d3d11.h>
#include "Rasterizer.h"
#include "Renderer.h"
#include "BindableCodex.h"

namespace Bindable
{
	Rasterizer::Rasterizer(bool two_sided)
		: m_bTwoSided(two_sided)
	{
		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = two_sided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
	
		m_pRasterizer = Graphics::Renderer::GetDevice()->CreateRasterizer(*reinterpret_cast<Graphics::RASTERIZER_DESC*>(&rasterDesc));
	}

	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(m_bTwoSided);
	}

	Ref<Rasterizer> Rasterizer::Resolve(bool two_sided)
	{
		return Codex::Resolve<Rasterizer>(two_sided);
	}

	std::string Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}

	void Rasterizer::Bind() noxnd
	{
		Graphics::Renderer::GetRenderContext()->BindRasterizerState(m_pRasterizer);
	}
}