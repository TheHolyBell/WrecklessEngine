#pragma once
#include "Bindable.h"
#include "Vertex.h"
#include "IShader.h"
#include "IInputLayout.h"

namespace Bindable
{
	class InputLayout : public IBindable
	{
	public:
		InputLayout(
			Dynamic::VertexLayout layout,
			void* compiled_shader);
		virtual void Bind() noxnd override;
		static Ref<InputLayout> Resolve(const Dynamic::VertexLayout& layout, void* compiled_shader);
		static std::string GenerateUID(const Dynamic::VertexLayout& layout, void* compiled_shader);
		std::string GetUID() const noexcept override;
	protected:
		Dynamic::VertexLayout m_Layout;
		Ref<Graphics::IInputLayout> m_pInputLayout;
	};
}