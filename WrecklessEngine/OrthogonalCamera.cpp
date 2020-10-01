#include "OrthogonalCamera.h"

namespace CameraSystem
{
	OrthogonalCamera::OrthogonalCamera()
	{
		DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_Proj, DirectX::XMMatrixIdentity());
	}
	DirectX::XMVECTOR OrthogonalCamera::GetPosition() const
	{
		return DirectX::XMLoadFloat3(&m_Position);
	}
	DirectX::XMFLOAT3 OrthogonalCamera::GetPosition3f() const
	{
		return m_Position;
	}
	void OrthogonalCamera::SetPosition(float x, float y, float z)
	{
		m_Position = { x,y,z };
	}
	void OrthogonalCamera::SetPosition(const DirectX::XMFLOAT3& v)
	{
		m_Position = v;
	}
	DirectX::XMVECTOR OrthogonalCamera::GetRight() const
	{
		return DirectX::XMLoadFloat3(&m_Right);
	}
	DirectX::XMFLOAT3 OrthogonalCamera::GetRight3f() const
	{
		return m_Right;
	}
	DirectX::XMVECTOR OrthogonalCamera::GetUp() const
	{
		return DirectX::XMLoadFloat3(&m_Up);
	}
	DirectX::XMFLOAT3 OrthogonalCamera::GetUp3f() const
	{
		return m_Up;
	}
	DirectX::XMVECTOR OrthogonalCamera::GetLook() const
	{
		return DirectX::XMLoadFloat3(&m_Look);
	}
	DirectX::XMFLOAT3 OrthogonalCamera::GetLook3f() const
	{
		return m_Look;
	}
	float OrthogonalCamera::GetNearZ() const
	{
		return m_NearZ;
	}
	float OrthogonalCamera::GetFarZ() const
	{
		return m_FarZ;
	}
	float OrthogonalCamera::GetLeftPlane() const
	{
		return m_LeftPlane;
	}
	float OrthogonalCamera::GetRightPlane() const
	{
		return m_RightPlane;
	}
	float OrthogonalCamera::GetTopPlane() const
	{
		return m_TopPlane;
	}
	float OrthogonalCamera::GetBottomPlane() const
	{
		return m_BottomPlane;
	}
	void OrthogonalCamera::SetFrustumProperties(float left, float right, float bottom, float top, float zn, float zf)
	{
		m_LeftPlane = left;
		m_RightPlane = right;
		m_TopPlane = top;
		m_BottomPlane = bottom;
		m_NearZ = zn;
		m_FarZ = zf;

		DirectX::XMStoreFloat4x4(&m_Proj, DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zn, zf));
	}
	void OrthogonalCamera::SetFrustumProperties(float width, float height, float zn, float zf)
	{
		m_LeftPlane = -1;
		m_RightPlane = width;
		m_TopPlane = -1;
		m_BottomPlane = height;
		m_NearZ = zn;
		m_FarZ = zf;

		DirectX::XMStoreFloat4x4(&m_Proj, DirectX::XMMatrixOrthographicLH(width, height, zn, zf));
	}
	void OrthogonalCamera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
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

	void OrthogonalCamera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
	{
		DirectX::XMVECTOR P = XMLoadFloat3(&pos);
		DirectX::XMVECTOR T = XMLoadFloat3(&target);
		DirectX::XMVECTOR U = XMLoadFloat3(&up);

		LookAt(P, T, U);

		m_bViewDirty = true;
	}
	DirectX::XMMATRIX OrthogonalCamera::GetView()const
	{
		assert(!m_bViewDirty);
		return DirectX::XMLoadFloat4x4(&m_View);
	}

	DirectX::XMMATRIX OrthogonalCamera::GetProjection()const
	{
		return DirectX::XMLoadFloat4x4(&m_Proj);
	}


	DirectX::XMFLOAT4X4 OrthogonalCamera::GetView4x4f()const
	{
		assert(!m_bViewDirty);
		return m_View;
	}

	DirectX::XMFLOAT4X4 OrthogonalCamera::GetProjection4x4f()const
	{
		return m_Proj;
	}

	void OrthogonalCamera::Strafe(float d)
	{
		// mPosition += d*mRight
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&m_Right);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, r, p));

		m_bViewDirty = true;
	}

	void OrthogonalCamera::Walk(float d)
	{
		// mPosition += d*mLook
		DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
		DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&m_Look);
		DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, l, p));

		m_bViewDirty = true;
	}

	void OrthogonalCamera::Pitch(float angle)
	{
		// Rotate up and look vector about the right vector.

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&m_Right), angle);

		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void OrthogonalCamera::Yaw(float angle)
	{
		// Rotate the basis vectors about the world y-axis.

		DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

		DirectX::XMStoreFloat3(&m_Right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Right), R));
		DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Up), R));
		DirectX::XMStoreFloat3(&m_Look, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Look), R));

		m_bViewDirty = true;
	}

	void OrthogonalCamera::UpdateViewMatrix()
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