#include "TransformCBuf.h"
#include "Renderer.h"
#include "SceneCamera.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"

namespace Bindable
{
	RefUnique<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::s_pVCBuf;

	TransformCBuf::TransformCBuf(UINT entID, UINT slot)
		: m_entID(entID)
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
		auto& tfComponent = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)m_entID).GetComponent<ECS::TransformComponent>();
		
		auto model = DirectX::XMLoadFloat4x4(&tfComponent.Transform);
		auto view = CameraSystem::SceneCamera::GetView();
		auto projection = CameraSystem::SceneCamera::GetProjection();
		Transforms tf = {};
		tf.model = DirectX::XMMatrixTranspose(model);
		tf.modelViewProjection = DirectX::XMMatrixTranspose(model *
			view * projection);

		return tf;
	}

	void TransformCBuf::Bind() noxnd
	{
		UpdateBindImpl(GetTransforms());
	}
}