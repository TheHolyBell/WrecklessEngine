#pragma once

#include <entt.hpp>


namespace ECS
{
	class Entity;

	class Scene
	{
	public :
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = {});

		void OnUpdate();
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}