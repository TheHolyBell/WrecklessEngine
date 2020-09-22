#pragma once

#include <string>
#include <DirectXMath.h>

#include "ScriptObject.h"
#include "ScriptClass.h"
#include "IMesh.h"

namespace ECS
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag){}
	};

	struct TransformComponent
	{
		DirectX::XMFLOAT4X4 Transform = {};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const DirectX::XMFLOAT4X4& transform)
			: Transform(transform) {}

		TransformComponent(DirectX::XMMATRIX matrix)
		{
			DirectX::XMStoreFloat4x4(&Transform, matrix);
		}

		operator DirectX::XMFLOAT4X4& () { return Transform; }
		operator const DirectX::XMFLOAT4X4& () const { return Transform; }
	};

	class ScriptComponent
	{
	public:
		ScriptComponent(Scripting::ScriptClass klass)
			: m_Object(klass.CreateInstance())
		{
		}
		void Update()
		{
			m_Object.Invoke("Update");
		}
	private:
		void OnCreate(uint32_t handle)
		{
			m_Object.GetProperty("ID").Set(handle);
			m_Object.Invoke("Start");
			m_bCreated = true;
		}
	private:
		bool m_bCreated = false;
		Scripting::ScriptObject m_Object;
		friend class Scene;
	};

	struct MeshComponent
	{
		Ref<Drawables::IMesh> pMesh;
		MeshComponent() = default;
		MeshComponent(Ref<Drawables::IMesh> mesh)
			: pMesh(mesh){}
	};
}