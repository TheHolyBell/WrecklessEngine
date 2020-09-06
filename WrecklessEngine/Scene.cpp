#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace ECS
{
	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
	}
	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>("Dickson");
		return entity;
	}
	void Scene::OnUpdate()
	{
		auto view = m_Registry.view<ScriptComponent>();
		for (const auto& sc : view)
		{
			ScriptComponent& ent = view.get<ScriptComponent>(sc);
			ent.Update();
		}
	}
}
