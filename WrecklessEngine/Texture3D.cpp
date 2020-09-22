#include "Texture3D.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;

namespace Bindable
{
	Texture3D::Texture3D(const std::string& path, UINT slot)
		: m_Path(path), m_Slot(slot)
	{
		m_pTexture = Renderer::GetDevice()->CreateTexture3D(path);
	}

	void Texture3D::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindTexture2D(m_pTexture, SHADER_TYPE::Pixel, m_Slot);
	}

	std::string Texture3D::GetUID() const noexcept
	{
		return GenerateUID(m_Path, m_Slot);
	}

	void* Texture3D::NativePointer() const noexcept
	{
		return m_pTexture->GetNativePointer();
	}

	unsigned Texture3D::GetWidth() const noexcept
	{
		return m_pTexture->GetWidth();
	}

	unsigned Texture3D::GetHeight() const noexcept
	{
		return m_pTexture->GetHeight();
	}

	Ref<Texture3D> Texture3D::Resolve(const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture3D>(path, slot);
	}
	std::string Texture3D::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture3D).name() + "#"s + path + "#" + std::to_string(slot);
	}
}