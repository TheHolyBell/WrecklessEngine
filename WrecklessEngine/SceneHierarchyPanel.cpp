#include "SceneHierarchyPanel.h"
#include "ImGui/imgui.h"
#include "Components.h"
#include "Entity.h"
#include "FileDialog.h"
#include "FileHelper.h"
#include "ScriptingEngine.h"
#include "ScriptField.h"
#include "Manipulators.h"

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
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponentPanel");

			if (ImGui::BeginPopup("AddComponentPanel"))
			{
				if (!m_SelectionContext.HasComponent<ECS::ScriptComponent>())
				{
					if (ImGui::Button("Script"))
					{
						m_SelectionContext.AddComponent<ECS::ScriptComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectionContext.HasComponent<ECS::ShadowCasterComponent>())
				{
					if (ImGui::Button("Shadow caster"))
					{
						m_SelectionContext.AddComponent<ECS::ShadowCasterComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}
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

	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16];

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	static void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}


	static void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}

	static bool Property(const char* label, std::string& value, bool error = false)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy(buffer, value.c_str());

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		if (error)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		if (ImGui::InputText(s_IDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}
		if (error)
			ImGui::PopStyleColor();
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static void Property(const char* label, const char* value)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	static bool Property(const char* label, bool& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::Checkbox(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, int& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, float& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, DirectX::XMFLOAT2& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat2(s_IDBuffer, &value.x, delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, DirectX::XMFLOAT3& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat3(s_IDBuffer, &value.x, delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, DirectX::XMFLOAT4& value, float delta = 0.1f)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat4(s_IDBuffer, &value.x, delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, ECS::Entity entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			bool removeComponent = false;

			auto& component = entity.GetComponent<T>();
			bool open = ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(T).hash_code()), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap, name.c_str());
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::NextColumn();
				ImGui::Columns(1);
				ImGui::TreePop();
			}
			ImGui::Separator();

			if (removeComponent)
				entity.RemoveComponent<T>();
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

		DrawComponent<ECS::ShadowCasterComponent>("Shadow caster", entity, [=](ECS::ShadowCasterComponent& scriptComponent) {});

		DrawComponent<ECS::ScriptComponent>("Script", entity, [=](ECS::ScriptComponent& scriptComponent) mutable
		{
			using namespace Scripting;

			/*static char class_name[1000];

			strcpy(class_name, scriptComponent.GetTypeName().c_str());*/



			BeginPropertyGrid();


			if (Property("Class name", scriptComponent.TypeName, !ScriptingEngine::GetDomain().IsClassExists(scriptComponent.TypeName)))
			{
				//IO::cout << "Changed" << IO::endl;
				if (ScriptingEngine::GetDomain().IsClassExists(scriptComponent.TypeName))
				{
					scriptComponent = ECS::ScriptComponent(ScriptingEngine::GetDomain().GetClass(scriptComponent.TypeName));
				}
			}

			if (ScriptingEngine::GetDomain().IsClassExists(scriptComponent.TypeName))
			{
				auto scriptClass = ScriptingEngine::GetDomain().GetClass(scriptComponent.TypeName);

				for (const auto& fieldName : scriptClass.GetFieldNames())
				{
					auto& field = scriptComponent.m_Object.GetField(fieldName);
					auto fieldType = field.GetType();
					switch (fieldType)
					{
					case FieldType::Int:
					{
						int value = field.GetValue<int>();
						if (Property(fieldName.c_str(), value))
							field.SetValue<int>(value);
						break;
					}
					case FieldType::Float:
					{
						float value = field.GetValue<float>();
						if (Property(fieldName.c_str(), value))
							field.SetValue<float>(value);
						break;
					}
					case FieldType::Vec2:
					{
						DirectX::XMFLOAT2 value = field.GetValue<DirectX::XMFLOAT2>();
						if (Property(fieldName.c_str(), value))
							field.SetValue<DirectX::XMFLOAT2>(value);
						break;
					}
					case FieldType::Vec3:
					{
						DirectX::XMFLOAT3 value = field.GetValue<DirectX::XMFLOAT3>();
						if (Property(fieldName.c_str(), value))
							field.SetValue<DirectX::XMFLOAT3>(value);
						break;
					}
					case FieldType::Vec4:
					{
						DirectX::XMFLOAT4 value = field.GetValue<DirectX::XMFLOAT4>();
						if (Property(fieldName.c_str(), value))
							field.SetValue<DirectX::XMFLOAT4>(value);
						break;
					}
					}
				}
			}
			EndPropertyGrid();
		});
	}
}