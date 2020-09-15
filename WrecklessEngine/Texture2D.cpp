#include "Texture2D.h"
#include "Renderer.h"
#include "BindableCodex.h"

using namespace Graphics;

namespace Bindable
{
	Texture2D::Texture2D(const std::string& path, UINT slot)
		: m_Path(path), m_Slot(slot)
	{
		m_pTexture = Renderer::GetDevice()->CreateTexture2D(path);
	}

	void Texture2D::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindTexture2D(m_pTexture, SHADER_TYPE::Pixel, m_Slot);
	}

	std::string Texture2D::GetUID() const noexcept
	{
		return GenerateUID(m_Path, m_Slot);
	}

	void* Texture2D::NativePointer() const noexcept
	{
		return m_pTexture->GetNativePointer();
	}

	unsigned Texture2D::GetWidth() const noexcept
	{
		return m_pTexture->GetWidth();
	}

	unsigned Texture2D::GetHeight() const noexcept
	{
		return m_pTexture->GetHeight();
	}

	Ref<Texture2D> Texture2D::Resolve(const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture2D>(path, slot);
	}
	std::string Texture2D::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture2D).name() + "#"s + path + "#" + std::to_string(slot);
	}
}