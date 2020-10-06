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

		virtual void OnResize(int width, int height) override;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		float GetSnapValue();

	private:
		Ref<Bindable::Texture2D> m_CheckerboardTex;
		Ref<Bindable::Texture2D> m_PlayButtonTex;
		Ref<Bindable::Texture2D> m_PauseButtonTex;

		Ref<ECS::Scene> m_pScene;

		bool m_ViewportPanelMouseOver = false;
		bool m_ViewportPanelFocused = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Pause = 2
		};

		struct ViewportSize
		{
			float Left = 0;
			float Top = 0;

			float Right = 0;
			float Bottom = 0;
		};

		ViewportSize m_ViewportDimensions = {};

		SceneState m_SceneState = SceneState::Edit;

		int m_GizmoType = -1;
		int m_GizmoTransformationType = 0;

		DirectX::XMFLOAT2 m_ViewportSize = {};

		Ref<SceneHierarchyPanel> m_pSceneHierarchyPanel;

		CameraSystem::PerspectiveCamera m_EditorCamera;
		float m_CameraSpeed = 80;
	};
}