#pragma once

#include <string>
#include <DirectXMath.h>

#include "ScriptObject.h"
#include "ScriptClass.h"
#include "ScriptingEngine.h"
#include "IMesh.h"
#include "Cubemap.h"

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

		TransformComponent() : TransformComponent(DirectX::XMMatrixIdentity())
		{}
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
		ScriptComponent()
			: ScriptComponent(Scripting::ScriptingEngine::GetDomain().GetClass("Sandbox.Actor"))
		{

		}

		ScriptComponent(Scripting::ScriptClass klass)
			: m_Object(klass.CreateInstance())
		{
			TypeName = m_Object.GetTypeName();
		}

		std::string TypeName;
		Scripting::ScriptObject m_Object;

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
		friend class Scene;
	};

	struct MeshComponent
	{
		Ref<Drawables::IMesh> Mesh;
		MeshComponent() = default;
		MeshComponent(Ref<Drawables::IMesh> mesh)
			: Mesh(mesh){}
	};

	struct CubemapComponent
	{
		Ref<Drawables::Cubemap> Cubemap;
		CubemapComponent() = default;
		CubemapComponent(Ref<Drawables::Cubemap> cubemap)
			 : Cubemap(cubemap) {}
	};

	struct ShadowCasterComponent
	{
		// Just a simple dummy-class for turning on-off shadow casting for entities
	private:
		char dummy; // Dummy member-field
	public:
		ShadowCasterComponent() = default;
	};
}