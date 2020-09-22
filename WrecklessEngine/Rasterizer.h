#pragma once
#include "Bindable.h"
#include "IRasterizer.h"

namespace Bindable
{
	class Rasterizer : public IBindable
	{
	public:
		Rasterizer(bool two_sided);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;

		virtual ~Rasterizer() = default;

		static Ref<Rasterizer> Resolve(bool two_sided);
		static std::string GenerateUID(bool two_sided);
	protected:
		Ref<Graphics::IRasterizer> m_pRasterizer;
		bool m_bTwoSided = false;
	};
}