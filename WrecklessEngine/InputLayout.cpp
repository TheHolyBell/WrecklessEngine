#include "InputLayout.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;

namespace Bindable
{
	InputLayout::InputLayout(Dynamic::VertexLayout layout, void* compiled_shader)
		: m_Layout(std::move(layout))
	{
		m_pInputLayout = Renderer::GetDevice()->CreateInputLayout(m_Layout, compiled_shader);
	}
	Ref<InputLayout> InputLayout::Resolve(const Dynamic::VertexLayout& layout, void* compiled_shader)
	{
		return Codex::Resolve<InputLayout>(layout, compiled_shader);
	}

	std::string InputLayout::GenerateUID(const Dynamic::VertexLayout& layout, void* compiled_shader)
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		return GenerateUID(m_Layout, nullptr);
	}


	void InputLayout::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindInputLayout(m_pInputLayout);
	}
}