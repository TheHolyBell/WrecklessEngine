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

	/*int type = mono_type_get_type(monoType);
	switch (type)
	{
	case MONO_TYPE_R4: return ComponentType::Float;
	case MONO_TYPE_I4: return ComponentType::Int;
	case MONO_TYPE_U4: return ComponentType::UnsignedInt;
	case MONO_TYPE_STRING: return ComponentType::String;
	case MONO_TYPE_CLASS:
	{
		char* name = mono_type_get_name(monoType);
		if (strcmp(name, "TagComponent") == 0) return ComponentType::Vec2;
		if (strcmp(name, "TransformComponent") == 0) return ComponentType::Vec3;
		if (strcmp(name, "Hazel.Vector4") == 0) return ComponentType::Vec4;
	}
	}
	return ComponentType::None;*/
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
