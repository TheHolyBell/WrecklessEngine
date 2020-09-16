#pragma once
#include "CommonInclude.h"

namespace Drawables
{
	class IMesh
	{
	public:
		virtual void Update() PURE;
		virtual void Draw() PURE;
		virtual ~IMesh() = default;
	};
}