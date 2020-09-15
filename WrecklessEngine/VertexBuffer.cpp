#include "VertexBuffer.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;

namespace Bindable
{
	VertexBuffer::VertexBuffer(const Dynamic::VertexBuffer& vbuf)
		:
		VertexBuffer("?", vbuf)
	{}

	VertexBuffer::VertexBuffer(const std::string& tag, const Dynamic::VertexBuffer& vbuf)
		:
		m_Stride((UINT)vbuf.GetLayout().Size()),
		m_Tag(tag),
		m_Layout(vbuf.GetLayout())
	{
		m_pVertexBuffer =  Renderer::GetDevice()->CreateVertexBuffer(vbuf);
	}

	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID(m_Tag);
	}

	const Dynamic::VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return m_Layout;
	}

	Ref<VertexBuffer> VertexBuffer::Resolve(const std::string& tag, const Dynamic::VertexBuffer& vbuf)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(tag, vbuf);
	}

	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}

	void VertexBuffer::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindVertexBuffer(m_pVertexBuffer, m_Stride, 0);
	}
}