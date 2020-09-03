#pragma once
#include "InfoManager.h"
#include <wrl.h>
#include <vector>
#include <dxgidebug.h>

namespace Graphics
{
	class DXGIInfoManager : public InfoManager
	{
	public:
		DXGIInfoManager();
		virtual ~DXGIInfoManager() = default;
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
		virtual void Set() noexcept override;
		virtual std::vector<std::string> GetMessages() const override;
	private:
		unsigned long long m_Next = 0;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_pDXGIInfoQueue;
	};
}