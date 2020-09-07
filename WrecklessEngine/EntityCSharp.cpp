#include "EntityCSharp.h"

#include "ScriptingEngine.h"
#include "SceneManager.h"
#include "Entity.h"

#include <mono/metadata/assembly.h>

#include "Components.h"

/*
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddComponent_Native(uint entityID, Type type);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasComponent_Native(uint entityID, Type type);
*/

enum ComponentType
{
	TagComponent,
	TransformComponent
};

ComponentType GetType(MonoObject* type)
{
	ComponentType _Type;

	MonoClass* klass = mono_object_get_class(type);

	Scripting::String str((MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr));

	if (std::strcmp(str.ToUTF8(), "TagComponent") == 0)
		_Type = TagComponent;
	else if (std::strcmp(str.ToUTF8(), "TransformComponent") == 0)
		_Type = TransformComponent;

	return _Type;
}

static void AddComponent_Native(unsigned int entityID, MonoObject* type)
{
	ECS::Entity entity =  ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID);

	switch (GetType(type))
	{
	case TagComponent:
		entity.AddComponent<ECS::TagComponent>();
		break;
	case TransformComponent:
		entity.AddComponent<ECS::TransformComponent>();
		break;
	}
}

static bool HasComponent_Native(unsigned int entityID, MonoObject* type)
{
	ECS::Entity entity = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID);

	switch (GetType(type))
	{
	case TagComponent:
		return entity.HasComponent<ECS::TagComponent>();
	case TransformComponent:
		return entity.HasComponent<ECS::TransformComponent>();
	}
}

namespace Scripting
{
	void EntityCSharp::Bind()
	{
		Scripting::ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Entity::AddComponent_Native", AddComponent_Native);
		Scripting::ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Entity::HasComponent_Native", HasComponent_Native);
	}
}
