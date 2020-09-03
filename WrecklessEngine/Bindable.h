#pragma once
#include "CommonInclude.h"
#include <string>
#include <memory>
#include "InfoManager.h"

namespace Graphics
{
	class Renderer;
}

namespace Bindable
{
	// @ represents bindable entity
	class IBindable
	{
	public:
		virtual void Bind(Graphics::Renderer& renderer) noxnd PURE;
		virtual std::string GetUID() const noexcept PURE;
		virtual ~IBindable() = default;
	protected:
		static std::shared_ptr<Graphics::InfoManager> GetInfoManager(Graphics::Renderer& renderer);
	};
}