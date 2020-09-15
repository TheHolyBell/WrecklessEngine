#pragma once
#include "Bindable.h"
#include "BindableCodex.h"
#include "IBuffer.h"
#include "Renderer.h"

namespace Bindable
{
	template<typename C>
	class ConstantBuffer : public IBindable
	{
	public:
		void Update(const C& data)
		{
			Graphics::Renderer::GetRenderContext()->MapDataToBuffer(m_pConstantBuffer, &data, sizeof(C));
		}
		ConstantBuffer(const C& data, UINT slot = 0u)
			:
			m_Slot(slot)
		{
			m_pConstantBuffer = Graphics::Renderer::GetDevice()->CreateConstantBuffer((const void*)&data, sizeof(C));
		}
		ConstantBuffer(UINT slot = 0u)
			:
			m_Slot(slot)
		{
			m_pConstantBuffer = Graphics::Renderer::GetDevice()->CreateConstantBuffer(sizeof(C));
		}
	protected:
		Ref<Graphics::IConstantBuffer> m_pConstantBuffer;
		UINT m_Slot;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_pConstantBuffer;
		using ConstantBuffer<C>::m_Slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		virtual void Bind() noxnd override
		{
			Graphics::Renderer::GetRenderContext()->BindConstantBuffer(m_pConstantBuffer, Graphics::SHADER_TYPE::Vertex, m_Slot);
		}
		static std::shared_ptr<VertexConstantBuffer> Resolve(const C& consts, UINT slot = 0)
		{
			return Codex::Resolve<VertexConstantBuffer>(consts, slot);
		}
		static std::shared_ptr<VertexConstantBuffer> Resolve(UINT slot = 0)
		{
			return Codex::Resolve<VertexConstantBuffer>(slot);
		}
		static std::string GenerateUID(const C&, UINT slot)
		{
			return GenerateUID(slot);
		}
		static std::string GenerateUID(UINT slot = 0)
		{
			using namespace std::string_literals;
			return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
		}
		virtual std::string GetUID() const noexcept override
		{
			return GenerateUID(m_Slot);
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_pConstantBuffer;
		using ConstantBuffer<C>::m_Slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		virtual void Bind() noxnd override
		{
			Graphics::Renderer::GetRenderContext()->BindConstantBuffer(m_pConstantBuffer, Graphics::SHADER_TYPE::Pixel, m_Slot);
		}
		static std::shared_ptr<PixelConstantBuffer> Resolve(const C& consts, UINT slot = 0)
		{
			return Codex::Resolve<PixelConstantBuffer>(consts, slot);
		}
		static std::shared_ptr<PixelConstantBuffer> Resolve(UINT slot = 0)
		{
			return Codex::Resolve<PixelConstantBuffer>(slot);
		}
		static std::string GenerateUID(const C&, UINT slot)
		{
			return GenerateUID(slot);
		}
		static std::string GenerateUID(UINT slot = 0)
		{
			using namespace std::string_literals;
			return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
		}
		std::string GetUID() const noexcept override
		{
			return GenerateUID(m_Slot);
		}
	};
}