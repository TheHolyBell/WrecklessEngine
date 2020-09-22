#include "Sampler.h"
#include "Renderer.h"
#include "BindableCodex.h"
#include <d3d11.h>

namespace Bindable
{
	Sampler::Sampler(Type type, bool reflect)
		: m_Type(type), m_bReflect(reflect)
	{

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = [type]() {
			switch (type)
			{
			case Type::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
			case Type::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
			default:
			case Type::Bilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}();
		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_pSamplerState = Graphics::Renderer::GetDevice()->CreateSamplerState(*reinterpret_cast<Graphics::SAMPLER_DESC*>(&samplerDesc));
	}

	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID(m_Type, m_bReflect);
	}
	std::string Sampler::GenerateUID(Type type, bool reflect)
	{
		using namespace std::string_literals;
		return typeid(Sampler).name() + "#"s + std::to_string((int)type) + (reflect ? "R"s : "W"s);
	}
	std::shared_ptr<Sampler> Sampler::Resolve(Type type, bool reflect)
	{
		return Codex::Resolve<Sampler>(type, reflect);
	}

	void Sampler::Bind() noxnd
	{
		Graphics::Renderer::GetRenderContext()->BindSamplerState(m_pSamplerState, 0);
	}
}