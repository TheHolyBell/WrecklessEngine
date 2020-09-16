#pragma once

namespace Graphics
{
	class Pipeline
	{
	public:
		static void Initialize(unsigned width, unsigned height);
		static void Execute();
		static void ResizeBuffers(unsigned width, unsigned height);
	};
}