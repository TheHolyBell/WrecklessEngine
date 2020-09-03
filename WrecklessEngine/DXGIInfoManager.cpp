#include "Timer.h"
#include "DXGIInfoManager.h"
#include "HrException.h"
#include "GraphicsThrowMacros.h"
#include "WindowsThrowMacros.h"
#include "Renderer.h"

namespace Graphics
{
	DXGIInfoManager::DXGIInfoManager()
	{
		// Define function signature of DXGIGetDebugInterface
		using DXGIGetDebugInterface = HRESULT(WINAPI*)(REFIID, void**);

		// Load the dll that contains the function DXGIGetDebugInterface
		const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (hModDxgiDebug == nullptr)
			throw WRECK_LAST_EXCEPT();

		// Get address of DXGIGetDebugInterface in DLL
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
			);

		if (DxgiGetDebugInterface == nullptr)
			throw WRECK_LAST_EXCEPT();

		HRESULT hr;
		GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_pDXGIInfoQueue));
	}
	void DXGIInfoManager::Set() noexcept
	{
		// Set the index (next) so that hte next all to GetMessages()
		// will only get error generated after this call
		m_Next = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}
	std::vector<std::string> DXGIInfoManager::GetMessages() const
	{
		std::vector<std::string> _Messages;
		const auto end = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = m_Next; i < end; i++)
		{
			HRESULT hr;
			SIZE_T messageLength;
			// get the size of message i in bytes
			GFX_THROW_NOINFO(m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
			// allocate memory for message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			// get the message and push its description into the vector
			GFX_THROW_NOINFO(m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
			_Messages.emplace_back(pMessage->pDescription);
		}
		return _Messages;
	}
}