#include "EditorLayer.h"
#include "ImGui/imgui.h"
#include "KeyCode.h"
#include "ImGui/ImGuizmo.h"
#include "Renderer.h"
#include "FileDialog.h"
#include "FileHelper.h"
#include "GlobalClock.h"
#include "ImGuiLogger.h"
#include "Manipulators.h"
#include "Application.h"

#include "VanillaPass.h"
#include "ShadowPass.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"

#include "GamePadCSharp.h"
#include "MouseCSharp.h"
#include "KeyboardCSharp.h"
#include "DebugConsoleCSharp.h"
#include "TimeCSharp.h"
#include "EntityCSharp.h"
#include "NoiseCSharp.h"
#include "ComponentsCSharp.h"
#include "Cubemap.h"

#include "Terrain.h"
#include "SceneCamera.h"

#include "Model.h"

#include "Keyboard.h"
#include "ScriptingEngine.h"

using namespace Input;

namespace Wreckless
{
	EditorLayer::EditorLayer()
	{
		Scripting::ScriptingEngine::Initialize("Sandbox.dll");

		m_pScene = std::make_shared<ECS::Scene>("main");
		ECS::SceneManager::AddScene(m_pScene);

		m_pSceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>(m_pScene);

		Scripting::Debug::Bind();
		Scripting::GamePadCSharp::Bind();
		Scripting::MouseCSharp::Bind();
		Scripting::KeyboardCSharp::Bind();
		Scripting::TimeCSharp::Bind();
		Scripting::NoiseCSharp::Bind();
		Scripting::ComponentsCSharp::Bind();
		Scripting::EntityCSharp::Bind();

		m_EditorCamera.LookAt(DirectX::XMFLOAT3{ 10.0f, 5.0f, -20.0f }, DirectX::XMFLOAT3{ 0,0,0 },
			DirectX::XMFLOAT3{ 0,1,0 });

		auto ent = m_pScene->CreateEntity();
		ent.AddComponent<ECS::TagComponent>("Boxie");
		ent.AddComponent<ECS::ScriptComponent>(Scripting::ScriptingEngine::GetDomain().GetClass("Sandbox.Actor"));
		ent.AddComponent<ECS::TransformComponent>(DirectX::XMMatrixTranslation(5.0, 2.0f, 10.0f));
		ent.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::TestCube>(ent.GetID(), 10));

		auto terrain = m_pScene->CreateEntity();
		terrain.AddComponent<ECS::TagComponent>("Terrain");
		terrain.AddComponent<ECS::TransformComponent>();
		terrain.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::Terrain>(terrain.GetID()));

		/*auto model = m_pScene->CreateEntity();
		model.AddComponent<ECS::TagComponent>("Wolf");
		model.AddComponent<ECS::TransformComponent>();
		model.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::Model>(model.GetID(), "assets/Models/Wolf/Wolf.fbx"));*/

		/*auto cubemap = m_pScene->CreateEntity();
		cubemap.AddComponent<ECS::TagComponent>("Cubemap");
		cubemap.AddComponent<ECS::CubemapComponent>(std::make_shared<Drawables::Cubemap>(cubemap.GetID(), "assets/Textures/sunsetcube1024.dds"));*/
	}
	EditorLayer::~EditorLayer()
	{
	}
	void EditorLayer::OnAttach()
	{
		// ImGui Colors
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f); // Window background
		colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.5f); // Widget backgrounds
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.4f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.6f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
		colors[ImGuiCol_Header] = ImVec4(0.7f, 0.7f, 0.7f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.8f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.5f, 0.52f, 1.0f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	
		m_CheckerboardTex = Bindable::Texture2D::Resolve("assets/Textures/Checkerboard.png");
		m_PlayButtonTex = Bindable::Texture2D::Resolve("assets/Textures/PlayButton.png");
		m_PauseButtonTex = Bindable::Texture2D::Resolve("assets/Textures/PauseButton.png");
	}
	void EditorLayer::OnDetach()
	{
	}
	void EditorLayer::OnUpdate()
	{
		Profiling::GlobalClock::Update();
		if (m_SceneState == SceneState::Play)
		{
			ECS::SceneManager::GetActiveScene()->OnUpdate();
		}
		using namespace Input;
		using namespace DirectX;

		auto mouseCoords = Mouse::GetPosition();
		static auto lastPos = mouseCoords;

		float delta = Profiling::GlobalClock::DeltaTime();
		auto rStickCoords = GamePad::Get().RightStick();
		auto lStickCoords = GamePad::Get().LeftStick();
		m_EditorCamera.Pitch(-XMConvertToRadians(rStickCoords.Y * delta * 400));
		m_EditorCamera.Yaw(XMConvertToRadians(rStickCoords.X * delta * 400));
		m_EditorCamera.Translate(lStickCoords.X * delta, 0.0f, lStickCoords.Y * delta);
		auto checker = [this](int mouseX, int mouseY)
		{
			if (mouseX >= m_ViewportDimensions.Left && mouseX <= m_ViewportDimensions.Right && mouseY >= m_ViewportDimensions.Top && mouseY <= m_ViewportDimensions.Bottom)
				return true;
			return false;
		};

		if (Mouse::IsMiddleMouseDown() && checker(mouseCoords.X, mouseCoords.Y))
		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = XMConvertToRadians(0.25f * static_cast<float>(mouseCoords.X - lastPos.X));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(mouseCoords.Y - lastPos.Y));

			m_EditorCamera.Pitch(dy);
			m_EditorCamera.Yaw(dx);


			if (Keyboard::IsKeyDown(KeyCode::W))
				m_EditorCamera.Walk(delta * m_CameraSpeed);
			if (Keyboard::IsKeyDown(KeyCode::S))
				m_EditorCamera.Walk(-delta * m_CameraSpeed);

			if (Keyboard::IsKeyDown(KeyCode::D))
				m_EditorCamera.Strafe(delta * m_CameraSpeed);
			if (Keyboard::IsKeyDown(KeyCode::A))
				m_EditorCamera.Strafe(-delta * m_CameraSpeed);

		}
		m_EditorCamera.UpdateViewMatrix();

		lastPos = mouseCoords;

		CameraSystem::SceneCamera::SetPosition(m_EditorCamera.GetPosition3f());
		CameraSystem::SceneCamera::SetView(m_EditorCamera.GetView());
		CameraSystem::SceneCamera::SetProjection(m_EditorCamera.GetProjection());
	}

	void EditorLayer::OnResize(int width, int height)
	{
		m_EditorCamera.SetFrustumProperties(3.14159 / 2, (float)width / (float)height, 0.1f, 1000);
		m_EditorCamera.UpdateViewMatrix();
	}

	void EditorLayer::OnImGuiRender()
	{
		WRECK_PROFILE_FUNCTION();


		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
					IO::cout << *FileSystem::FileDialog::OpenFile() << IO::endl;
				if (ImGui::MenuItem("Save"))
					IO::cout << *FileSystem::FileDialog::SaveFile() << IO::endl;
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Data"))
			{
				if (ImGui::MenuItem("Clear World"))
				{
					m_pScene->Clear();
					m_pSceneHierarchyPanel->ResetFocus();
					IO::cout << "Clear World command has been issued" << IO::endl;
				}

				if (ImGui::MenuItem("Load Mesh"))
				{
					auto file_path = FileSystem::FileDialog::OpenFile();
					if (file_path)
					{
						auto model = m_pScene->CreateEntity();
						auto file_name = FileSystem::FileHelper::GetFileNameFromPath(*file_path);
						model.AddComponent<ECS::TagComponent>(file_name);
						model.AddComponent<ECS::TransformComponent>();
						model.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::Model>(model.GetID(), *file_path));
						m_pSceneHierarchyPanel->SetFocus(model);
					}
				}



				if (ImGui::MenuItem("Load Cubemap"))
				{
					auto file_path = FileSystem::FileDialog::OpenFile();
					if (file_path)
					{
						auto cubemap = m_pScene->CreateEntity();
						auto file_name = FileSystem::FileHelper::GetFileNameFromPath(*file_path);
						cubemap.AddComponent<ECS::TagComponent>(file_name);
						cubemap.AddComponent<ECS::CubemapComponent>(std::make_shared<Drawables::Cubemap>(cubemap.GetID(), *file_path));
						m_pSceneHierarchyPanel->SetFocus(cubemap);
					}
				}

				ImGui::EndMenu();
			}

			static bool vsync = false;
			if (ImGui::BeginMenu("Renderer Settings"))
			{
				if (ImGui::MenuItem("VSync", nullptr, &vsync))
					Application::GetInstance().GetWindow().SetVSyncState(vsync);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		
		using namespace Graphics;
		if (ImGui::Begin("Renderer"))
		{
			auto& caps = Renderer::GetCapabilities();
			ImGui::Text("GPU: %s", caps.GPU_Name.c_str());
			ImGui::Text("Memory size: %d MB", caps.Memory_Size);
			ImGui::Text("Delta Time: %.5fs", Profiling::GlobalClock::DeltaTime());
			ImGui::Text("Total Time: %.2fs", Profiling::GlobalClock::TotalTime());
			ImGui::Text("Framerate: %d", Profiling::GlobalClock::GetFrameCount());
			ImGui::Text("Image dimensions: %dx%d", m_CheckerboardTex->GetWidth(), m_CheckerboardTex->GetHeight());
			ImGui::Image((ImTextureID)m_CheckerboardTex->NativePointer(), ImVec2(m_CheckerboardTex->GetWidth(), m_CheckerboardTex->GetHeight()));
			ImGui::Image((ImTextureID)m_PlayButtonTex->NativePointer(), ImVec2(m_PlayButtonTex->GetWidth(), m_PlayButtonTex->GetHeight()));
			//ImGui::Image((ImTextureID)VanillaPass::GetDepthStencilSRV()->GetNativePointer(), ImVec2(400, 200));
			ImGui::Image((ImTextureID)ShadowPass::GetDepthStencilSRV()->GetNativePointer(), ImVec2(400, 400));
		}
		ImGui::End();
		IO::ImGuiOutput::Draw(m_SceneState == SceneState::Play);

		m_pSceneHierarchyPanel->OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::Begin("Toolbar");

		switch (m_SceneState)
		{
		case SceneState::Edit:
			if (ImGui::ImageButton((ImTextureID)(m_PlayButtonTex->NativePointer()), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
			{
				m_SceneState = SceneState::Play;
				Profiling::GlobalClock::Reset();
				IO::ImGuiOutput::Clear();
				IO::cout << "Play" << IO::endl;
			}
			break;
		case SceneState::Play:
			if (ImGui::ImageButton((ImTextureID)(m_PauseButtonTex->NativePointer()), ImVec2(m_PauseButtonTex->GetWidth(), m_PauseButtonTex->GetHeight()), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
			{
				m_SceneState = SceneState::Edit;
				IO::cout << "Edit" << IO::endl;
			}
			break;
		}

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);
		
		m_ViewportPanelMouseOver = ImGui::IsWindowHovered();
		m_ViewportPanelFocused = ImGui::IsWindowFocused();
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)VanillaPass::GetRenderTargetSRV()->GetNativePointer(), viewportPanelSize);
		
		m_ViewportDimensions.Left = ImGui::GetWindowPos().x;
		m_ViewportDimensions.Top = ImGui::GetWindowPos().y;
		m_ViewportDimensions.Right = ImGui::GetWindowWidth() + m_ViewportDimensions.Left;
		m_ViewportDimensions.Bottom = ImGui::GetWindowHeight() + m_ViewportDimensions.Top;

		if (m_GizmoType != -1 && m_pSceneHierarchyPanel->HasSelectedEntity())
		{
			ECS::Entity selectedEntity = m_pSceneHierarchyPanel->GetSelectedEntity();

			if (selectedEntity.HasComponent<ECS::TransformComponent>())
			{
				DirectX::XMFLOAT4X4 view;
				DirectX::XMFLOAT4X4 projection;
				DirectX::XMStoreFloat4x4(&view, CameraSystem::SceneCamera::GetView());
				DirectX::XMStoreFloat4x4(&projection, CameraSystem::SceneCamera::GetProjection());

				float width = ImGui::GetWindowWidth();
				float height = ImGui::GetWindowHeight();
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

				bool snap = Input::Keyboard::IsKeyDown(Input::KeyCode::LControl);

				auto& entityTransform = selectedEntity.GetComponent<ECS::TransformComponent>().Transform;
				float snapValue = GetSnapValue();
				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(reinterpret_cast<const float*>(&view),
					reinterpret_cast<const float*>(&projection),
					(ImGuizmo::OPERATION)m_GizmoType,
					(ImGuizmo::MODE)m_GizmoTransformationType,
					reinterpret_cast<float*>(&entityTransform),
					nullptr,
					snap ? snapValues : nullptr);
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
		ImGui::Begin("StatusBar", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

		if ((m_ViewportSize.x != viewportPanelSize.x) || (viewportPanelSize.y != m_ViewportSize.y))
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			VanillaPass::Resize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}
	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(WRECK_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(WRECK_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (m_ViewportPanelFocused)
		{
			switch (e.GetKeyCode())
			{
			case KeyCode::Q:
				m_GizmoType = -1;
				break;
			case KeyCode::T:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case KeyCode::R:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case KeyCode::S:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			case KeyCode::L:
				m_GizmoTransformationType = ImGuizmo::LOCAL;
				break;
			case KeyCode::W:
				m_GizmoTransformationType = ImGuizmo::WORLD;
				break;
			}
		}

		if (e.GetKeyCode() == KeyCode::Delete)
		{
			if (m_pSceneHierarchyPanel->HasSelectedEntity())
			{
				auto entity = m_pSceneHierarchyPanel->GetSelectedEntity();
				m_pScene->DestroyEntity(entity);
				m_pSceneHierarchyPanel->ResetFocus();
			}
		}
		return false;
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}
	float EditorLayer::GetSnapValue()
	{
		switch (m_GizmoType)
		{
		case ImGuizmo::OPERATION::TRANSLATE:
			return 0.5f;
		case ImGuizmo::OPERATION::ROTATE:
			return 45.0f;
		case ImGuizmo::OPERATION::SCALE: 
			return 0.5f;
		}
		return 0.0f;
	}
}