#include "Pipeline.h"
#include "VanillaPass.h"

namespace Graphics
{
	void Pipeline::Initialize(unsigned width, unsigned height)
	{
		VanillaPass::Initialize(width, height);
	}
	void Pipeline::Execute()
	{
		VanillaPass::Execute();
	}
	void Pipeline::ResizeBuffers(unsigned width, unsigned height)
	{
		VanillaPass::Resize(width, height);
	}
}