#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "InputOutputStream.h"
#include "Manipulators.h"

namespace ECS
{
	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
	}
	Scene::~Scene()
	{
		IO::cout << m_Name << " has been destroyed" << IO::endl;
	}
	std::string Scene::GetName() const noexcept
	{
		return m_Name;
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		//entity.AddComponent<TransformComponent>();
		//auto& tag = entity.AddComponent<TagComponent>("Dickson");
		return entity;
	}
	Entity Scene::GetEntityByIndex(entt::entity handle)
	{
		WRECK_ASSERT(EntityExists(handle), "Entity doesn't exists");
		return Entity(handle, this);
	}
	bool Scene::EntityExists(entt::entity handle) const
	{
		return m_Registry.valid(handle);
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		if(m_Registry.valid(entity.m_Entity))
			m_Registry.destroy(entity.m_Entity);
	}

	void Scene::Clear()
	{
		m_Registry.clear();
	}

	void Scene::OnUpdate()
	{
		auto view = QueryElementsByComponent<ScriptComponent>();
		for (const auto& ent : view)
		{
			ScriptComponent& sc = view.get<ScriptComponent>(ent);
			if (sc.m_bCreated == false)
				sc.OnCreate((uint32_t)ent);
			sc.Update();
		}
	}
}
