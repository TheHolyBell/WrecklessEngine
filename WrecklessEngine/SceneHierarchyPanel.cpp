#include "SceneHierarchyPanel.h"
#include "ImGui/imgui.h"
#include "Components.h"
#include "Entity.h"
#include "FileDialog.h"
#include "FileHelper.h"

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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = { entt::null, nullptr };

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);
		ImGui::End();
	}
	bool SceneHierarchyPanel::HasSelectedEntity() const
	{
		return (bool)m_SelectionContext;
	}
	ECS::Entity SceneHierarchyPanel::GetSelectedEntity() const
	{
		return m_SelectionContext;
	}
	void SceneHierarchyPanel::SetFocus(ECS::Entity entity)
	{
		m_SelectionContext = entity;
	}
	void SceneHierarchyPanel::ResetFocus()
	{
		m_SelectionContext = {entt::null, nullptr};
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
	void SceneHierarchyPanel::DrawComponents(ECS::Entity entity)
	{
		ImGui::AlignTextToFramePadding();

		if (entity.HasComponent<ECS::TagComponent>())
		{
			auto& tag = entity.GetComponent<ECS::TagComponent>().Tag;

			char buffer[256] = {};
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = buffer;
			ImGui::Separator();
		}


		if (entity.HasComponent<ECS::TransformComponent>())
		{
			if(ImGui::TreeNodeEx((void*)typeid(ECS::TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<ECS::TransformComponent>().Transform;
				ImGui::DragFloat3("Position", &transform._41, 1, -FLT_MAX, FLT_MAX);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<ECS::CubemapComponent>())
		{
			auto& cubemap = entity.GetComponent<ECS::CubemapComponent>();
			
			if (ImGui::Button("Reload"))
			{
				auto file_path = FileSystem::FileDialog::OpenFile();
				if (file_path.has_value())
				{
					entity.RemoveComponent<ECS::CubemapComponent>();
					ECS::CubemapComponent& cc = entity.AddComponent<ECS::CubemapComponent>( std::make_shared<Drawables::Cubemap>(entity.GetID(), file_path.value()));
					entity.GetComponent<ECS::TagComponent>().Tag = FileSystem::FileHelper::GetFileNameFromPath(file_path.value());
				}
			}
		}
	}
}