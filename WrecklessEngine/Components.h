#pragma once

#include <string>
#include <DirectXMath.h>

#include "ScriptObject.h"
#include "ScriptClass.h"

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
		DirectX::XMFLOAT4X4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const DirectX::XMFLOAT4X4& transform)
			: Transform(transform) {}

		operator DirectX::XMFLOAT4X4& () { return Transform; }
		operator const DirectX::XMFLOAT4X4& () const { return Transform; }
	};

	class ScriptComponent
	{
	public:
		ScriptComponent(uint32_t id, Scripting::ScriptClass klass)
			: m_Object(klass.CreateInstance())
		{
			m_Object.Invoke("Start");
			m_Object.GetProperty("ID").Set(id);
		}
		void Update()
		{
			m_Object.Invoke("Update");
		}

		Scripting::ScriptObject& Object()
		{
			return m_Object; 
		}
	private:
		Scripting::ScriptObject m_Object;
	};
}