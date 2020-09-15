#pragma once
#include "Bindable.h"
#include "Vertex.h"
#include "IBuffer.h"

namespace Bindable
{
	class VertexBuffer : public IBindable
	{
	public:
		VertexBuffer(const std::string& tag, const Dynamic::VertexBuffer& vbuf);
		VertexBuffer(const Dynamic::VertexBuffer& vbuf);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;
		const Dynamic::VertexLayout& GetLayout() const noexcept;
	
		static Ref<VertexBuffer> Resolve(const std::string& tag,
			const Dynamic::VertexBuffer& vbuf);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string m_Tag;
		UINT m_Stride;
		Ref<Graphics::IVertexBuffer> m_pVertexBuffer;
		Dynamic::VertexLayout m_Layout;
	};
}