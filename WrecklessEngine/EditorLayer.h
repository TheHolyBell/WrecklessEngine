#pragma once
#include "CommonInclude.h"
#include "Layer.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "PerspectiveCamera.h"

#include "TestCube.h"
#include "Scene.h"
#include "ScriptDomain.h"
#include "SceneHierarchyPanel.h"

namespace Wreckless
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		Ref<Bindable::Texture2D> m_CheckerboardTex;
		Ref<Bindable::Texture2D> m_PlayButtonTex;
		Ref<Bindable::Texture2D> m_PauseButtonTex;

		Ref<Bindable::Texture2D> m_pTexture;
		Ref<Bindable::Texture3D> m_pCubemap;

		Ref<ECS::Scene> m_pScene;
		Scripting::ScriptDomain m_Domain;

		bool m_ViewportPanelMouseOver = false;
		bool m_ViewportPanelFocused = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Pause = 2
		};

		SceneState m_SceneState = SceneState::Edit;

		int m_GizmoType = -1;

		DirectX::XMFLOAT2 m_ViewportSize = {};

		Ref<SceneHierarchyPanel> m_pSceneHierarchyPanel;

		CameraSystem::PerspectiveCamera m_EditorCamera;
	};
}