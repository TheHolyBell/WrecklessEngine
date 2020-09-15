#pragma once
#include "Bindable.h"
#include "IBuffer.h"

namespace Bindable
{
	class IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(const std::vector<unsigned>& indices);
		IndexBuffer(const std::string& tag, const std::vector<unsigned>& indices);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;
		UINT GetCount() const noexcept;

		static Ref<IndexBuffer> Resolve(const std::string& tag,
			const std::vector<unsigned>& indices);

		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string m_Tag;
		UINT m_Count;
		Ref<Graphics::IIndexBuffer> m_pIndexBuffer;
	};
}