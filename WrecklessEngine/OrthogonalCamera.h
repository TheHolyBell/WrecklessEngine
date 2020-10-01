#pragma once
#include <DirectXMath.h>

namespace CameraSystem
{
	class OrthogonalCamera
	{
	public:
		OrthogonalCamera();

		// Get/Set world camera position
		DirectX::XMVECTOR GetPosition() const;
		DirectX::XMFLOAT3 GetPosition3f() const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const DirectX::XMFLOAT3& v);

		// Get camera basis vectors
		DirectX::XMVECTOR GetRight() const;
		DirectX::XMFLOAT3 GetRight3f() const;

		DirectX::XMVECTOR GetUp() const;
		DirectX::XMFLOAT3 GetUp3f() const;

		DirectX::XMVECTOR GetLook() const;
		DirectX::XMFLOAT3 GetLook3f() const;

		// Get frustum properties
		float GetNearZ() const;
		float GetFarZ() const;
		float GetLeftPlane() const;
		float GetRightPlane() const;
		float GetTopPlane() const;
		float GetBottomPlane() const;

		// Set frustum
		void SetFrustumProperties(float left, float right, float bottom, float top, float zn, float zf);
		void SetFrustumProperties(float width, float height, float zn, float zf);

		// Define camera space via LookAt parameters
		void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
		void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target,
			const DirectX::XMFLOAT3& up);

		// Get View/Proj matrices
		DirectX::XMMATRIX GetView() const;
		DirectX::XMMATRIX GetProjection() const;

		DirectX::XMFLOAT4X4 GetView4x4f() const;
		DirectX::XMFLOAT4X4 GetProjection4x4f() const;

		// Strafe/Walk the camera a distance d
		void Walk(float d);
		void Strafe(float d);

		// Rotate the camera
		void Pitch(float angle);
		void Yaw(float angle);

		// After modifying camera position/orientation, call
		void UpdateViewMatrix();
	private:
		// Camera coordinate system with coordinates relative to world space
		DirectX::XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_Right = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_Up = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_Look = { 0.0f, 0.0f, 0.0f };

		// Cache frustum properties
		float m_NearZ = 0.0f;
		float m_FarZ = 0.0f;
		float m_LeftPlane = 0.0f;
		float m_RightPlane = 0.0f;
		float m_TopPlane = 0.0f;
		float m_BottomPlane = 0.0f;
		// Cache View/Proj matrices
		DirectX::XMFLOAT4X4 m_View;
		DirectX::XMFLOAT4X4 m_Proj;

		bool m_bViewDirty = true;

	};
}