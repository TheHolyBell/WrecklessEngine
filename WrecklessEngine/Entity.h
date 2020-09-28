#pragma once

#include <entt.hpp>
#include "CommonInclude.h"
#include "Scene.h"

namespace ECS
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(entt::entity handle, Scene* pScene);
		Entity(const Entity& rhs) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			WRECK_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			return m_pScene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			WRECK_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return m_pScene->m_Registry.get<T>(m_Entity);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_pScene->m_Registry.has<T>(m_Entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			WRECK_ASSERT(HasComponent<T>(), "Entity doesn't has component!");
			m_pScene->m_Registry.remove<T>(m_Entity);
		}

		operator bool() const { return m_Entity != entt::null; }
		operator uint32_t() const { return (uint32_t)m_Entity; }

		uint32_t GetID() const { return (uint32_t)m_Entity; }

		bool operator==(const Entity& rhs) const
		{
			return m_Entity == rhs.m_Entity && m_pScene == rhs.m_pScene;
		}

		bool operator!=(const Entity& rhs) const
		{
			return !((*this)==rhs);
		}
	private:
		entt::entity m_Entity = entt::null;
		Scene* m_pScene = nullptr;

		friend Scene;
	};
}