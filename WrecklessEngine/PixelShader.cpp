#include "PixelShader.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;

namespace Bindable
{
	PixelShader::PixelShader(const std::string& path)
	{
		m_pShader = Renderer::GetDevice()->CreatePixelShader(path);
	}

	void* PixelShader::GetByteCode() const noexcept
	{
		return m_pShader->GetByteCode();
	}

	Ref<PixelShader> PixelShader::Resolve(const std::string& path)
	{
		return Codex::Resolve<PixelShader>("Shaders/Bin/" + path);
	}

	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}

	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(m_Path);
	}

	void PixelShader::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindPixelShader(m_pShader);
	}
}