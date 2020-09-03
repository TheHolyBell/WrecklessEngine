#pragma once

#include "CommonInclude.h"
#include <Windows.h>
#include "WrecklessException.h"

namespace Graphics
{
	class IWindow
	{
	public:
		virtual void* GetWindowHandle() const PURE;
		virtual int GetWidth() const PURE;
		virtual int GetHeight() const PURE;
		virtual void SetTitle(const char* title) PURE;
		virtual void SetIcon(const char* filename) PURE;
		virtual ~IWindow() = default;
	};
}