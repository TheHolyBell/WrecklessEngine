#pragma once

#include "IWindow.h"
#include <Windows.h>

namespace Graphics
{
	class Win32Window : public IWindow
	{
	public:
		Win32Window(const char* Title, int Width, int Height);

		virtual void* GetWindowHandle() const override;
		virtual int GetWidth() const override;
		virtual int GetHeight() const override;
		virtual void SetTitle(const char* title) override;

		virtual ~Win32Window() = default;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND m_hWnd;
		int m_Width;
		int m_Height;
	};
}