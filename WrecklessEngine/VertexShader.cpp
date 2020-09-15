#include "VertexShader.h"
#include "Renderer.h"
#include "BindableCodex.h"

using namespace Graphics;

namespace Bindable
{
	VertexShader::VertexShader(const std::string& path)
		: m_Path(path)
	{
		m_pShader = Renderer::GetDevice()->CreateVertexShader(path);
	}


	void* VertexShader::GetByteCode() const noexcept
	{
		return m_pShader->GetByteCode();
	}

	Ref<VertexShader> VertexShader::Resolve(const std::string& path)
	{
		return Codex::Resolve<VertexShader>("Shaders/Bin/" + path);
	}

	std::string VertexShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}

	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(m_Path);
	}

	void VertexShader::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindVertexShader(m_pShader);
	}
}