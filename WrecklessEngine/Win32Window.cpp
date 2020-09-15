#include "Win32Window.h"
#include "HrException.h"
#include "WindowsThrowMacros.h"
#include "Keyboard.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ImGui/imgui_impl_win32.h"

#include "Renderer.h"

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
    void Win32Window::SetIcon(const char* filename)
    {
        auto _hIcon = (HICON)LoadImage(nullptr, filename, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
        auto _hIconSmall = (HICON)LoadImage(nullptr, filename, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
        SendMessageA(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)_hIcon);
        SendMessageA(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)_hIcon);
    }
    void Win32Window::OnUpdate()
    {
        Graphics::Renderer::GetSwapChain()->SwapBuffers(m_bVSync ? Graphics::SwapFlags::RATE_60 : Graphics::SwapFlags::NO_LIMIT);
    }
    void Win32Window::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Callback = callback;
    }
    void Win32Window::SetVSyncState(bool state)
    {
        m_bVSync = state;
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
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return 1;

        switch (msg)
        {
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            Input::Keyboard::KeyPressed((Input::KeyCode)wParam);
            KeyPressedEvent event((Input::KeyCode)wParam);
            m_Callback(event);
            break;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        { 
            Input::Keyboard::KeyReleased((Input::KeyCode)wParam);
            KeyReleasedEvent event((Input::KeyCode)wParam);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_CHAR:
        {
            KeyTypedEvent event((Input::KeyCode)wParam);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_SIZE:
        {
            m_Width = LOWORD(lParam);
            m_Height = HIWORD(lParam);
            WindowResizeEvent event(m_Width, m_Height);
            if (m_Callback != nullptr && !m_bResizing)
                m_Callback(event);
            break;
        }

        case WM_ENTERSIZEMOVE:
            m_bResizing = true;
            break;

        case WM_EXITSIZEMOVE:
        {
            m_bResizing = false;
            WindowResizeEvent event(m_Width, m_Height);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            MouseButtonPressedEvent event(Input::KeyCode::LButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            MouseButtonPressedEvent event(Input::KeyCode::RButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            MouseButtonPressedEvent event(Input::KeyCode::MButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_LBUTTONUP:
        {
            MouseButtonReleasedEvent event(Input::KeyCode::LButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }
        case WM_RBUTTONUP:
        {
            MouseButtonReleasedEvent event(Input::KeyCode::RButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }
        case WM_MBUTTONUP:
        {
            MouseButtonReleasedEvent event(Input::KeyCode::MButton);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_MOUSEWHEEL:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            MouseScrolledEvent event(pt.x, pt.y);
            if (m_Callback != nullptr)
                m_Callback(event);
            break;
        }

        case WM_CLOSE:
        case WM_QUIT:
            WindowCloseEvent event;
            if (m_Callback != nullptr)
                m_Callback(event);
            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

}