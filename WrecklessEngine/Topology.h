#pragma once
#include "Bindable.h"
#include "TopologyTypes.h"

namespace Bindable
{
	class Topology : public IBindable
	{
	public:
		Topology(Graphics::PRIMITIVE_TOPOLOGY topology);
		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;
	
		static Ref<Topology> Resolve(Graphics::PRIMITIVE_TOPOLOGY topology = Graphics::PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static std::string GenerateUID(Graphics::PRIMITIVE_TOPOLOGY topology);

	protected:
		Graphics::PRIMITIVE_TOPOLOGY m_Topology;
	};
}