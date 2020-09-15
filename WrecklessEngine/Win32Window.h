#pragma once

#include "CommonInclude.h"
#include "IWindow.h"
#include <Windows.h>
#include "Event.h"

namespace Graphics
{
	class Win32Window : public IWindow
	{
	public:
		//using EventCallbackFn = std::function<void(Event&)>;

		Win32Window(const char* Title, int Width, int Height);

		virtual void* GetWindowHandle() const override;
		virtual int GetWidth() const override;
		virtual int GetHeight() const override;
		virtual void SetTitle(const char* title) override;
		virtual void SetIcon(const char* filename) override;
		virtual void OnUpdate() override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;
		virtual void SetVSyncState(bool state) override;
		virtual ~Win32Window() = default;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND m_hWnd;
		EventCallbackFn m_Callback = nullptr;
		int m_Width;
		int m_Height;
		bool m_bVSync = false;
		bool m_bResizing = false;
	};
}