#pragma once
#include "Drawable.h"

namespace Drawables
{
	class TestCube : public Drawable
	{
	public:
		TestCube(unsigned entID, float size);
		void SetPos(DirectX::XMFLOAT3 pos) noexcept;
		void SetRotation(float roll, float pitch, float yaw) noexcept;
	
	private:
		DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
	};
}