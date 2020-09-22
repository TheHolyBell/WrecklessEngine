#pragma once
#include "CommonInclude.h"
#include "Scene.h"
#include "Entity.h"

namespace ECS
{
}

namespace Wreckless
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<ECS::Scene> scene);

		void SetContext(Ref<ECS::Scene> scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(ECS::Entity entity);
		void DrawComponents(ECS::Entity entity);
	private:
		Ref<ECS::Scene> m_Context;
		ECS::Entity m_SelectionContext;
	};
}