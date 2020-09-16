#pragma once
#include "CommonInclude.h"
#include "Renderer.h"
#include "IMesh.h"

namespace Bindable
{
	class IBindable;
	class IndexBuffer;
}

namespace Drawables
{
	class Drawable : public IMesh
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;

		virtual void Update() override;
		virtual void Draw() override final;
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