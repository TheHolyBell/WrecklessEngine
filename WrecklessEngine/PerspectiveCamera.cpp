#include "PerspectiveCamera.h"

namespace CameraSystem
{
	PerspectiveCamera::PerspectiveCamera()
	{
		DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_Proj, DirectX::XMMatrixIdentity());
	}
	DirectX::XMVECTOR PerspectiveCamera::GetPosition() const
	{
		return DirectX::XMLoadFloat3(&m_Position);
	}
	DirectX::XMFLOAT3 PerspectiveCamera::GetPosition3f() const
	{
		return m_Position;
	}
	void PerspectiveCamera::SetPosition(float x, float y, float z)
	{
		m_Position = { x,y,z };
	}
	void PerspectiveCamera::SetPosition(const DirectX::XMFLOAT3& v)
	{
		m_Position = v;
	}
	void PerspectiveCamera::Translate(float x, float y, float z)
	{
		using namespace DirectX;
		XMFLOAT3 translation = { x, y, z };
		
		XMStoreFloat3(&translation, XMVector3Transform(
			XMLoadFloat3(&translation),
			XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f) *
			XMMatrixScaling(80, 80, 80)
		));
		m_Position = {
			m_Position.x + translation.x,
			m_Position.y + translation.y,
			m_Position.z + translation.z
		};
	}
	DirectX::XMVECTOR PerspectiveCamera::GetRight() const
	{
		return DirectX::XMLoadFloat3(&m_Right);
	}
	DirectX::XMFLOAT3 PerspectiveCamera::GetRight3f() const
	{
		return m_Right;
	}
	DirectX::XMVECTOR PerspectiveCamera::GetUp() const
	{
		return DirectX::XMLoadFloat3(&m_Up);
	}
	DirectX::XMFLOAT3 PerspectiveCamera::GetUp3f() const
	{
		return m_Up;
	}
	DirectX::XMVECTOR PerspectiveCamera::GetLook() const
	{
		return DirectX::XMLoadFloat3(&m_Look);
	}
	DirectX::XMFLOAT3 PerspectiveCamera::GetLook3f() const
	{
		return m_Look;
	}
	float PerspectiveCamera::GetNearZ() const
	{
		return m_NearZ;
	}
	float PerspectiveCamera::GetFarZ() const
	{
		return m_FarZ;
	}
	float PerspectiveCamera::GetAspect() const
	{
		return m_Aspect;
	}
	float PerspectiveCamera::GetFovY() const
	{
		return m_FovY;
	}
	float PerspectiveCamera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();
		return 2.0f * atan(halfWidth / m_NearZ);
	}
	float PerspectiveCamera::GetNearWindowWidth() const
	{
		return m_Aspect * m_NearWindowHeight;
	}
	float PerspectiveCamera::GetNearWindowHeight() const
	{
		return m_NearWindowHeight;
	}
	float PerspectiveCamera::GetFarWindowWidth() const
	{
		return m_Aspect * m_FarWindowHeight;
	}
	float PerspectiveCamera::GetFarWindowHeight() const
	{
		return m_FarWindowHeight;
	}
	void PerspectiveCamera::SetFrustumProperties(float fovY, float aspect, float zn, float zf)
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
	void PerspectiveCamera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
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

	void PerspectiveCamera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
	{
		DirectX::XMVECTOR P = XMLoadFloat3(&pos);
		DirectX::XMVECTOR T = XMLoadFloat3(&target);
		DirectX::XMVECTOR U = XMLoadFloat3(&up);

		LookAt(P, T, U);

		m_bViewDirty = true;
	}
	DirectX::XMMATRIX PerspectiveCamera::GetView()const
	{
		assert(!m_bViewDirty);
		return DirectX::XMLoadFloat4x4(&m_View);
	}

	DirectX::XMMATRIX PerspectiveCamera::GetProjection()const
	{
		return DirectX::XMLoadFloat4x4(&m_Proj);
	}


	DirectX::XMFLOAT4X4 PerspectiveCamera::GetView4x4f()const
	{
		assert(!m_bViewDirty);
		return m_View;
	}

	DirectX::XMFLOAT4X4 PerspectiveCamera::GetProjection4x4f()const
	{
		return m_Proj;
	}

	void PerspectiveCamera::Strafe(float d)
	{
		// mPosition += d*mRight
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&m_Right);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, r, p));

		m_bViewDirty = true;
	}

	void PerspectiveCamera::Walk(float d)
	{
		// mPosition += d*mLook
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&m_Look);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, l, p));

		m_bViewDirty = true;
	}

	void PerspectiveCamera::Pitch(float angle)
	{
		// Rotate up and look vector about the right vector.
		m_Pitch += angle;
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&m_Right), angle);

		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void PerspectiveCamera::Yaw(float angle)
	{
		// Rotate the basis vectors about the world y-axis.
		m_Yaw += angle;
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

		DirectX::XMStoreFloat3(&m_Right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Right), R));
		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void PerspectiveCamera::UpdateViewMatrix()
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