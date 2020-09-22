#pragma once
#include "Drawable.h"

namespace Drawables
{
	class Cubemap : public Drawable
	{
	public:
		Cubemap(unsigned entID, const std::string& path);
	};
}