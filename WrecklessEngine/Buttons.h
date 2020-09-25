#pragma once

namespace Input
{
	enum class Button
	{
		ArrowUp = 0x0001,
		ArrowDown = 0x0002,
		ArrowLeft = 0x0004,
		ArrowRight = 0x0008,
		StartButton = 0x0010,
		BackButton = 0x0020,
		LThumb = 0x0040,
		RThumb = 0x0080,
		LShoulder = 0x0100,
		RShoudler = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
	};
}