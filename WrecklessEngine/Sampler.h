#pragma once
#include "Bindable.h"
#include "ISamplerState.h"

namespace Bindable
{
	class Sampler : public IBindable
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point,
		};
	public:
		Sampler(Type type, bool reflect);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;

		static Ref<Sampler> Resolve(Type type = Type::Anisotropic, bool reflect = false);
		static std::string GenerateUID(Type type, bool reflect);

		virtual ~Sampler() = default;
	private:
		Ref<Graphics::ISamplerState> m_pSamplerState;
		Type m_Type;
		bool m_bReflect;
	};
}