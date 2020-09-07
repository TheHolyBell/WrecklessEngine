#pragma once
#include "Scene.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace ECS
{
	class SceneManager
	{
	public:
		static void AddScene(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> GetActiveScene();
		static void SetActiveScene(const std::string& name);
	private:
		static std::shared_ptr<Scene> m_pActiveScene;
		static std::unordered_map<std::string, std::shared_ptr<Scene>> m_Scenes;
	};
}