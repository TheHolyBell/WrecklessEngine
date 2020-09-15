#pragma once

#include "CommonInclude.h"
#include <Windows.h>
#include "WrecklessException.h"
#include "Event.h"

namespace Graphics
{
	class IWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void* GetWindowHandle() const PURE;
		virtual int GetWidth() const PURE;
		virtual int GetHeight() const PURE;
		virtual void OnUpdate() PURE;
		virtual void SetTitle(const char* title) PURE;
		virtual void SetIcon(const char* filename) PURE;
		virtual void SetEventCallback(const EventCallbackFn& callback) PURE;
		virtual void SetVSyncState(bool state) PURE;
		virtual ~IWindow() = default;
	};
}