#include "EditorLayer.h"
#include "ImGui/imgui.h"
#include "KeyCode.h"
#include "ImGui/ImGuizmo.h"
#include "Renderer.h"
#include "FileDialog.h"
#include "GlobalClock.h"
#include "ImGuiLogger.h"
#include "Manipulators.h"
#include "Application.h"

#include "VanillaPass.h"
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

using namespace Input;

namespace Wreckless
{
	EditorLayer::EditorLayer()
		: m_Domain("Sandbox.dll")
	{
		m_pTexture = Bindable::Texture2D::Resolve("D:\\Downloads\\image.png");

		m_pScene = std::make_shared<ECS::Scene>("main");
		ECS::SceneManager::AddScene(m_pScene);

		Scripting::Debug::Bind();
		Scripting::GamePadCSharp::Bind();
		Scripting::MouseCSharp::Bind();
		Scripting::KeyboardCSharp::Bind();
		Scripting::TimeCSharp::Bind();
		Scripting::NoiseCSharp::Bind();
		Scripting::ComponentsCSharp::Bind();
		Scripting::EntityCSharp::Bind();

		auto ent = m_pScene->CreateEntity();
		ent.AddComponent<ECS::ScriptComponent>(ent.GetID(), m_Domain.GetClass("Sandbox", "Actor"));
		ent.AddComponent<ECS::TransformComponent>(DirectX::XMMatrixTranslation(5.0, 2.0f, 10.0f));
		ent.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::TestCube>(ent.GetID(), 10));
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
	}
	void EditorLayer::OnDetach()
	{
	}
	void EditorLayer::OnUpdate()
	{
	}

	void EditorLayer::OnImGuiRender()
	{
		WRECK_PROFILE_FUNCTION();


		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
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

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		//if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		//	window_flags |= ImGuiWindowFlags_NoBackground;

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

		static bool vsync = false;

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
					IO::cout << "Clear World command has been issued" << IO::endl;
				ImGui::EndMenu();
			}

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
			ImGui::Image((ImTextureID)VanillaPass::GetDepthStencilSRV()->GetNativePointer(), ImVec2(400, 200));
		}
		ImGui::End();
		IO::ImGuiOutput::Draw();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)VanillaPass::GetRenderTargetSRV()->GetNativePointer(), viewportPanelSize);
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
			case KeyCode::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case KeyCode::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case KeyCode::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}

		return false;
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}
}