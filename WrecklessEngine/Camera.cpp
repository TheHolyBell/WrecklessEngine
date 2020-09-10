#include "Camera.h"

namespace Graphics
{
	Camera::Camera()
	{
		DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_Proj, DirectX::XMMatrixIdentity());
	}
	DirectX::XMVECTOR Camera::GetPosition() const
	{
		return DirectX::XMLoadFloat3(&m_Position);
	}
	DirectX::XMFLOAT3 Camera::GetPosition3f() const
	{
		return m_Position;
	}
	void Camera::SetPosition(float x, float y, float z)
	{
		m_Position = { x,y,z };
	}
	void Camera::SetPosition(const DirectX::XMFLOAT3& v)
	{
		m_Position = v;
	}
	DirectX::XMVECTOR Camera::GetRight() const
	{
		return DirectX::XMLoadFloat3(&m_Right);
	}
	DirectX::XMFLOAT3 Camera::GetRight3f() const
	{
		return m_Right;
	}
	DirectX::XMVECTOR Camera::GetUp() const
	{
		return DirectX::XMLoadFloat3(&m_Up);
	}
	DirectX::XMFLOAT3 Camera::GetUp3f() const
	{
		return m_Up;
	}
	DirectX::XMVECTOR Camera::GetLook() const
	{
		return DirectX::XMLoadFloat3(&m_Look);
	}
	DirectX::XMFLOAT3 Camera::GetLook3f() const
	{
		return m_Look;
	}
	float Camera::GetNearZ() const
	{
		return m_NearZ;
	}
	float Camera::GetFarZ() const
	{
		return m_FarZ;
	}
	float Camera::GetAspect() const
	{
		return m_Aspect;
	}
	float Camera::GetFovY() const
	{
		return m_FovY;
	}
	float Camera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();
		return 2.0f * atan(halfWidth / m_NearZ);
	}
	float Camera::GetNearWindowWidth() const
	{
		return m_Aspect * m_NearWindowHeight;
	}
	float Camera::GetNearWindowHeight() const
	{
		return m_NearWindowHeight;
	}
	float Camera::GetFarWindowWidth() const
	{
		return m_Aspect * m_FarWindowHeight;
	}
	float Camera::GetFarWindowHeight() const
	{
		return m_FarWindowHeight;
	}
	void Camera::SetLens(float fovY, float aspect, float zn, float zf)
	{
		// Cache properties
		m_FovY = fovY;
		m_Aspect = aspect;
		m_NearZ = zn;
		m_FarZ = zf;

		m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
		m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

		DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
		DirectX::XMStoreFloat4x4(&m_Proj, P);
	}
	void Camera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
	{
		DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, pos));
		DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, L));
		DirectX::XMVECTOR U = DirectX::XMVector3Cross(L, R);

		DirectX::XMStoreFloat3(&m_Position, pos);
		DirectX::XMStoreFloat3(&m_Look, L);
		DirectX::XMStoreFloat3(&m_Right, R);
		DirectX::XMStoreFloat3(&m_Up, U);

		m_bViewDirty = true;
	}

	void Camera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
	{
		DirectX::XMVECTOR P = XMLoadFloat3(&pos);
		DirectX::XMVECTOR T = XMLoadFloat3(&target);
		DirectX::XMVECTOR U = XMLoadFloat3(&up);

		LookAt(P, T, U);

		m_bViewDirty = true;
	}
	DirectX::XMMATRIX Camera::GetView()const
	{
		assert(!m_bViewDirty);
		return DirectX::XMLoadFloat4x4(&m_View);
	}

	DirectX::XMMATRIX Camera::GetProjection()const
	{
		return DirectX::XMLoadFloat4x4(&m_Proj);
	}


	DirectX::XMFLOAT4X4 Camera::GetView4x4f()const
	{
		assert(!m_bViewDirty);
		return m_View;
	}

	DirectX::XMFLOAT4X4 Camera::GetProjection4x4f()const
	{
		return m_Proj;
	}

	void Camera::Strafe(float d)
	{
		// mPosition += d*mRight
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&m_Right);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, r, p));

		m_bViewDirty = true;
	}

	void Camera::Walk(float d)
	{
		// mPosition += d*mLook
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&m_Look);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, l, p));

		m_bViewDirty = true;
	}

	void Camera::Pitch(float angle)
	{
		// Rotate up and look vector about the right vector.

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&m_Right), angle);

		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void Camera::Yaw(float angle)
	{
		// Rotate the basis vectors about the world y-axis.

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

		DirectX::XMStoreFloat3(&m_Right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Right), R));
		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void Camera::UpdateViewMatrix()
	{
		if (m_bViewDirty)
		{
			DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&m_Right);
			DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&m_Up);
			DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&m_Look);
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&m_Position);

			// Keep camera's axes orthogonal to each other and of unit length.
			L = DirectX::XMVector3Normalize(L);
			U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

			// U, L already ortho-normal, so no need to normalize cross product.
			R = DirectX::XMVector3Cross(U, L);

			// Fill in the view matrix entries.
			float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
			float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
			float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

			DirectX::XMStoreFloat3(&m_Right, R);
			DirectX::XMStoreFloat3(&m_Up, U);
			DirectX::XMStoreFloat3(&m_Look, L);

			m_View(0, 0) = m_Right.x;
			m_View(1, 0) = m_Right.y;
			m_View(2, 0) = m_Right.z;
			m_View(3, 0) = x;

			m_View(0, 1) = m_Up.x;
			m_View(1, 1) = m_Up.y;
			m_View(2, 1) = m_Up.z;
			m_View(3, 1) = y;

			m_View(0, 2) = m_Look.x;
			m_View(1, 2) = m_Look.y;
			m_View(2, 2) = m_Look.z;
			m_View(3, 2) = z;

			m_View(0, 3) = 0.0f;
			m_View(1, 3) = 0.0f;
			m_View(2, 3) = 0.0f;
			m_View(3, 3) = 1.0f;

			m_bViewDirty = false;
		}
	}
}