#pragma once

namespace Graphics
{
	struct Viewport
	{
		float MinDepth;
		float MaxDepth;
		int TopLeftX;
		int TopLeftY;
		int Width;
		int Height;
	};
}