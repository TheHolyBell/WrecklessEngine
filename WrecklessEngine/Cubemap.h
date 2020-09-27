#pragma once
#include "Drawable.h"

namespace Drawables
{
	class Cubemap : public Drawable
	{
	public:
		Cubemap(unsigned entID, const std::string& path);
		std::string GetPath() const;
	private:
		std::string m_Path;
	};
}