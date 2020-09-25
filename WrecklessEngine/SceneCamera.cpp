#include "SceneCamera.h"

namespace CameraSystem
{
	DirectX::XMMATRIX SceneCamera::s_View = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX SceneCamera::s_Projeciton = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 SceneCamera::s_Position = {};

	DirectX::XMMATRIX SceneCamera::GetView()
	{
		return s_View;
	}

	DirectX::XMMATRIX SceneCamera::GetProjection()
	{
		return s_Projeciton;
	}

	DirectX::XMFLOAT3 SceneCamera::GetPosition()
	{
		return s_Position;
	}

	void SceneCamera::SetPosition(DirectX::XMFLOAT3 pos)
	{
		s_Position = pos;
	}

	void SceneCamera::SetView(DirectX::FXMMATRIX view)
	{
		s_View = view;
	}

	void SceneCamera::SetView(DirectX::XMFLOAT4X4 view)
	{
		s_View = DirectX::XMLoadFloat4x4(&view);
	}

	void SceneCamera::SetProjection(DirectX::FXMMATRIX projection)
	{
		s_Projeciton = projection;
	}

	void SceneCamera::SetProjection(DirectX::XMFLOAT4X4 projection)
	{
		s_Projeciton = DirectX::XMLoadFloat4x4(&projection);
	}

}