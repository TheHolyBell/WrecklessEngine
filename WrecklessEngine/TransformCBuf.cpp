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
		: m_EntID(entID)
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
		ECS::Entity entity = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)m_EntID);

		auto model = DirectX::XMMatrixIdentity();

		if(entity.HasComponent<ECS::TransformComponent>())
			model = DirectX::XMLoadFloat4x4(&entity.GetComponent<ECS::TransformComponent>().Transform);
		
		auto view = CameraSystem::SceneCamera::GetView();
		auto projection = CameraSystem::SceneCamera::GetProjection();
		Transforms tf = {};
		tf.model = DirectX::XMMatrixTranspose(model);
		tf.view = DirectX::XMMatrixTranspose(view);
		tf.projection = DirectX::XMMatrixTranspose(projection);

		return tf;
	}

	void TransformCBuf::Bind() noxnd
	{
		UpdateBindImpl(GetTransforms());
	}
}