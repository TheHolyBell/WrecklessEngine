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
		DirectX::XMMATRIX Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const DirectX::XMMATRIX& transform)
			: Transform(transform) {}

		operator DirectX::XMMATRIX& () { return Transform; }
		operator const DirectX::XMMATRIX& () const { return Transform; }
	};

	class ScriptComponent
	{
	public:
		ScriptComponent(Scripting::ScriptClass klass)
			: m_Object(klass.CreateInstance())
		{
			m_Object.Invoke("Start");
		}
		void Update()
		{
			m_Object.Invoke("Update");
		}
	private:
		Scripting::ScriptObject m_Object;
	};
}