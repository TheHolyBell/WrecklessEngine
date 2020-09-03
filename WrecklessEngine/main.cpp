#include <Windows.h>
#include "ConsoleClass.h"
#include "InputOutputStream.h"
#include "Manipulators.h"
#include "Win32Window.h"
#include <memory>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int)
{
	Console::Initialize();

	std::shared_ptr<Graphics::IWindow> pWindow = std::make_shared<Graphics::Win32Window>("Hello buddy", 800, 600);

	pWindow->SetTitle("Dickson");

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			IO::cout << "Window dimensions: " << pWindow->GetWidth() << "-" << pWindow->GetHeight() << IO::endl;
	}

	return 0;
}