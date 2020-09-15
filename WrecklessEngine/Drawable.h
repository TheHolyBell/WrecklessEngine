#pragma once
#include "CommonInclude.h"
#include "Renderer.h"

namespace Bindable
{
	class IBindable;
	class IndexBuffer;
}

namespace Misc
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
		void Draw() const noxnd;
		virtual ~Drawable() = default;
		template<class T>
		T* QueryBindable() noexcept
		{
			for (auto& pb : m_Binds)
			{
				if (auto pt = dynamic_cast<T*>(pb.get()))
				{
					return pt;
				}
			}
			return nullptr;
		}
	protected:
		void AddBind(Ref<Bindable::IBindable> bindable) noxnd;
	private:
		const Bindable::IndexBuffer* m_pIndexBuffer = nullptr;
		std::vector<Ref<Bindable::IBindable>> m_Binds;
	};
}