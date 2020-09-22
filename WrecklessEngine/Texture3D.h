#pragma once
#include "Bindable.h"
#include "ITexture.h"

namespace Bindable
{
	class Texture3D : public IBindable
	{
	public:
		Texture3D(const std::string& path, UINT slot = 0);

		virtual void Bind() noxnd override;
		virtual std::string GetUID() const noexcept override;

		void* NativePointer() const noexcept;
		unsigned GetWidth() const noexcept;
		unsigned GetHeight() const noexcept;

		static Ref<Texture3D> Resolve(const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
	private:
		UINT m_Slot;
		Ref<Graphics::ITexture> m_pTexture;
		std::string m_Path;
	};
}