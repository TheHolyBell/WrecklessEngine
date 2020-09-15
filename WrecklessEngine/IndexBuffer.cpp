#include "IndexBuffer.h"
#include "BindableCodex.h"
#include "Renderer.h"

using namespace Graphics;

namespace Bindable
{
	IndexBuffer::IndexBuffer(const std::vector<unsigned>& indices)
		:
		IndexBuffer("?", indices)
	{
	}
	IndexBuffer::IndexBuffer(const std::string& tag, const std::vector<unsigned>& indices)
		:
		m_Tag(tag),
		m_Count((UINT)indices.size())
	{
		m_pIndexBuffer = Renderer::GetDevice()->CreateIndexBuffer(indices);
	}

	void IndexBuffer::Bind() noxnd
	{
		Renderer::GetRenderContext()->BindIndexBuffer(m_pIndexBuffer, 0);
	}
	UINT IndexBuffer::GetCount() const noexcept
	{
		return m_Count;
	}
	Ref<IndexBuffer> IndexBuffer::Resolve(const std::string& tag,
		const std::vector<unsigned>& indices)
	{
		assert(tag != "?");
		return Codex::Resolve<IndexBuffer>(tag, indices);
	}
	std::string IndexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
	std::string IndexBuffer::GetUID() const noexcept
	{
		return GenerateUID_(m_Tag);
	}
}