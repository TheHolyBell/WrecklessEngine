#include "SceneManager.h"
#include "CommonInclude.h"

namespace ECS
{
	std::shared_ptr<Scene> SceneManager::m_pActiveScene;
	std::unordered_map<std::string, std::shared_ptr<Scene>> SceneManager::m_Scenes;

	void SceneManager::AddScene(std::shared_ptr<Scene> scene)
	{
		m_Scenes[scene->GetName()] = std::move(scene);
	}
	std::shared_ptr<Scene> SceneManager::GetActiveScene()
	{
		return m_pActiveScene;
	}
	void SceneManager::SetActiveScene(const std::string& name)
	{
		auto iter = m_Scenes.find(name);

		if (iter == m_Scenes.end())
			WRECK_ASSERT(false, "Couldn't find scene");

		m_pActiveScene = iter->second;
	}
}