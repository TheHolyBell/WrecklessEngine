#include "EntityCSharp.h"

#include "ScriptingEngine.h"
#include "SceneManager.h"
#include "Entity.h"

#include <mono/metadata/assembly.h>

#include "Components.h"


enum class ComponentType
{
	None,
	Int,
	Float,
	UnsignedInt,
	String,
	Vec2,
	Vec3,
	Vec4,

	TagComponent,
	TransformComponent
};

ComponentType GetType(MonoObject* monoType)
{
	ComponentType _Type = ComponentType::None;

	MonoClass* klass = mono_object_get_class(monoType);

	Scripting::String str((MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), monoType, nullptr, nullptr));

	if (std::strcmp(str.ToUTF8(), "TagComponent") == 0)
		_Type = ComponentType::TagComponent;
	else if (std::strcmp(str.ToUTF8(), "TransformComponent") == 0)
		_Type = ComponentType::TransformComponent;

	return _Type;
}

static void AddComponent_Native(unsigned int entityID, MonoObject* type)
{
	ECS::Entity entity =  ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID);

	switch (GetType(type))
	{
	case ComponentType::TagComponent:
		entity.AddComponent<ECS::TagComponent>();
		break;
	case ComponentType::TransformComponent:
		entity.AddComponent<ECS::TransformComponent>();
		break;
	}
}

static bool HasComponent_Native(unsigned int entityID, MonoObject* type)
{
	ECS::Entity entity = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID);

	switch (GetType(type))
	{
	case ComponentType::TagComponent:
		return entity.HasComponent<ECS::TagComponent>();
	case ComponentType::TransformComponent:
		return entity.HasComponent<ECS::TransformComponent>();
	}
}

static void RemoveComponent_Native(unsigned int entityID, MonoObject* type)
{
	ECS::Entity entity = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)entityID);

	switch (GetType(type))
	{
	case ComponentType::TagComponent:
		entity.RemoveComponent<ECS::TagComponent>();
		break;
	case ComponentType::TransformComponent:
		entity.RemoveComponent<ECS::TransformComponent>();
		break;
	}
}

namespace Scripting
{
	void EntityCSharp::Bind()
	{
		Scripting::ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Entity::AddComponent_Native", AddComponent_Native);
		Scripting::ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Entity::HasComponent_Native", HasComponent_Native);
		Scripting::ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Entity::RemoveComponent_Native", RemoveComponent_Native);
	}
}
