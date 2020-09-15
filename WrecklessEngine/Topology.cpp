#include "Topology.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;
namespace Bindable
{
	Topology::Topology(Graphics::PRIMITIVE_TOPOLOGY topology)
		: m_Topology(topology)
	{
	}

	void Topology::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindTopology(m_Topology);
	}

	std::string Topology::GetUID() const noexcept
	{
		return GenerateUID(m_Topology);
	}

	Ref<Topology> Topology::Resolve(Graphics::PRIMITIVE_TOPOLOGY topology)
	{
		return Codex::Resolve<Topology>(topology);
	}

	std::string Topology::GenerateUID(PRIMITIVE_TOPOLOGY topology)
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string(topology);
	}
}