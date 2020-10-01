#pragma once

#include <DirectXMath.h>

namespace CameraSystem
{
	class SceneCamera
	{
	public:
		static DirectX::XMMATRIX GetView();
		static DirectX::XMMATRIX GetProjection();
		
		static DirectX::XMFLOAT3 GetPosition();
		static void SetPosition(DirectX::XMFLOAT3 pos);

		static void SetView(DirectX::FXMMATRIX view);
		static void SetView(DirectX::XMFLOAT4X4 view);

		static void SetProjection(DirectX::FXMMATRIX projection);
		static void SetProjection(DirectX::XMFLOAT4X4 projection);

	private:
		static DirectX::XMMATRIX s_View;
		static DirectX::XMMATRIX s_Projeciton;
		static DirectX::XMFLOAT3 s_Position;
	};
}