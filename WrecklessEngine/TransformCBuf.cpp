#include "TransformCBuf.h"
#include "Renderer.h"
#include "SceneCamera.h"

namespace Bindable
{
	RefUnique<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::s_pVCBuf;

	TransformCBuf::TransformCBuf(UINT slot)
	{
		if (s_pVCBuf == nullptr)
			s_pVCBuf = std::make_unique<VertexConstantBuffer<Transforms>>(slot);
	}

	std::string TransformCBuf::GetUID() const noexcept
	{
		return "Dick";
	}

	void TransformCBuf::UpdateBindImpl(const Transforms& tf) noexcept
	{
		s_pVCBuf->Update(tf);
		s_pVCBuf->Bind();
	}

	TransformCBuf::Transforms TransformCBuf::GetTransforms() noexcept
	{
		auto view = CameraSystem::SceneCamera::GetView();
		auto projection = CameraSystem::SceneCamera::GetProjection();
		Transforms tf = {};
		tf.model = DirectX::XMMatrixIdentity();
		tf.modelViewProjection = DirectX::XMMatrixTranspose(
			view * projection);

		return tf;
	}

	void TransformCBuf::Bind() noxnd
	{
		UpdateBindImpl(GetTransforms());
	}
}