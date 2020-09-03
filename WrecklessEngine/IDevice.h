#pragma once

#include "CommonInclude.h"

namespace Graphics
{
	class IDevice
	{
	public:
		virtual void* GetDevicePointer() const PURE;

		virtual ~IDevice() = default;
	};
}