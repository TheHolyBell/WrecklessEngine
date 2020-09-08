#pragma once
#include "CommonInclude.h"
#include "Scene.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace ECS
{
	class SceneManager
	{
	public:
		static void AddScene(Ref<Scene> scene);
		static Ref<Scene> GetActiveScene();
		static void SetActiveScene(const std::string& name);
	private:
		static std::shared_ptr<Scene> m_pActiveScene;
		static std::unordered_map<std::string, Ref<Scene>> m_Scenes;
	};
}