#pragma once
#include "Bindable.h"
#include <DirectXMath.h>
#include "ConstantBuffers.h"
#include "Entity.h"

namespace Bindable
{
	class TransformCBuf : public IBindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};
	public:
		TransformCBuf(UINT entID, UINT slot = 0);
		virtual void Bind() noxnd override;

		virtual std::string GetUID() const noexcept override;
	protected:
		void UpdateBindImpl(const Transforms& tf) noexcept;
		Transforms GetTransforms() noexcept;
	private:
		static RefUnique<VertexConstantBuffer<Transforms>> s_pVCBuf;
		unsigned m_EntID;
	};
}