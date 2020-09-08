#include "ComponentsCSharp.h"
#include "Components.h"
#include "Entity.h"
#include "SceneManager.h"
#include "ScriptingEngine.h"

/*
		[MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetTag_Native(uint entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTag_Native(uint entityID, string value);

		------------------------------------------------------------------------

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void GetTransform_Native(uint entityID, out Matrix4 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void SetTransform_Native(uint entityID, ref Matrix4 result);
*/

static MonoString* GetTag_Native(unsigned int entityID)
{
	return Scripting::String(ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID).GetComponent<ECS::TagComponent>().Tag.c_str()).GetString();
}

static void SetTag_Native(unsigned int entityID, MonoString* string)
{
	ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID).GetComponent<ECS::TagComponent>().Tag = Scripting::String(string).ToUTF8();
}

static void GetTransform_Native(unsigned int entityID, DirectX::XMFLOAT4X4* result)
{
	memcpy(result, &ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID).GetComponent<ECS::TransformComponent>().Transform, sizeof(DirectX::XMFLOAT4X4));
}

static void SetTransform_Native(unsigned int entityID, DirectX::XMFLOAT4X4* result)
{
	ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID).GetComponent<ECS::TransformComponent>().Transform = *result;
}

namespace Scripting
{
	void ComponentsCSharp::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.TagComponent::GetTag_Native", GetTag_Native);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.TagComponent::SetTag_Native", SetTag_Native);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.TransformComponent::GetTransform_Native", GetTransform_Native);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.TransformComponent::SetTransform_Native", SetTransform_Native);
	}
}