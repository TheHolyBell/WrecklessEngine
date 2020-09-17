#include "SceneHierarchyPanel.h"
#include "ImGui/imgui.h"
#include "Components.h"
#include "Entity.h"

namespace Wreckless
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<ECS::Scene> scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(Ref<ECS::Scene> scene)
	{
		m_Context = scene;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				ECS::Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(ECS::Entity entity)
	{
		auto& tag = entity.GetComponent<ECS::TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		if (ImGui::TreeNodeEx((void*)entity.GetID(), flags, tag.c_str()))
		{
			if (ImGui::IsItemClicked())
				m_SelectionContext = entity;

			ImGui::TreePop();
		}
	}
}