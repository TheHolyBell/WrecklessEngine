#pragma once

#include <entt.hpp>


namespace ECS
{
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name);
		~Scene();

		std::string GetName() const noexcept;

		Entity CreateEntity(const std::string& name = {});

		Entity GetEntityByIndex(entt::entity handle);
		bool EntityExists(entt::entity handle) const;

		template<typename... Args>
		decltype(auto) QueryElementsByComponent()
		{
			return m_Registry.view<Args...>();
		}

		void OnUpdate();
	private:
		entt::registry m_Registry;
		std::string m_Name;
		friend class Entity;
	};
}