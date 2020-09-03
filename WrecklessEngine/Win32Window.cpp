#include "Win32Window.h"
#include "HrException.h"
#include "WindowsThrowMacros.h"

namespace Graphics
{
    Win32Window::Win32Window(const char* Title, int Width, int Height)
        : m_Width(Width), m_Height(Height)
    {
        static auto* className = TEXT("Win32Window");
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = className;
        
        RegisterClassEx(&wc);

        RECT rc = {};
        rc.right = Width;
        rc.bottom = Height;

        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        m_hWnd = CreateWindow(className, Title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
            rc.bottom - rc.top, nullptr, nullptr, wc.hInstance, this);

        if (m_hWnd == nullptr)
            throw WRECK_LAST_EXCEPT();

        ShowWindow(m_hWnd, SW_SHOW);

    }
    void* Win32Window::GetWindowHandle() const
    {
        return m_hWnd;
    }
    int Win32Window::GetWidth() const
    {
        return m_Width;
    }
    int Win32Window::GetHeight() const
    {
        return m_Height;
    }
    void Win32Window::SetTitle(const char* title)
    {
        SetWindowTextA(m_hWnd, title);
    }
    LRESULT Win32Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
        if (msg == WM_NCCREATE)
        {
            // extract ptr to window class from creation data
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Win32Window* const pWnd = static_cast<Win32Window*>(pCreate->lpCreateParams);
            // set WinAPI-managed user data to store ptr to window instance
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            // set message proc to normal (non-setup) handler now that setup is finished
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Win32Window::HandleMsgThunk));
            // forward message to window instance handler
            return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
        }
        // if we get a message before the WM_NCCREATE message, handle with default handler
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // Retrieve ptr to window instance
        Win32Window* const _pWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        // Forward message to window instance handler
        return _pWnd->HandleMsg(hwnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_SIZE:
        {
            m_Width = LOWORD(lParam);
            m_Height = HIWORD(lParam);
            break;
        }
        case WM_CLOSE:
        case WM_QUIT:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

}