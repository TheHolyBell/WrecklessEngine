#include "Entity.h"

namespace ECS
{
	Entity::Entity(entt::entity handle, Scene* pScene)
		: m_Entity(handle), m_pScene(pScene)
	{
	}
}
