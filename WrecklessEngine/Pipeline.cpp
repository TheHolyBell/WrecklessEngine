#include "Pipeline.h"
#include "VanillaPass.h"
#include "ShadowPass.h"

namespace Graphics
{
	void Pipeline::Initialize(unsigned width, unsigned height)
	{
		ShadowPass::Initialize(4096, 4096);
		VanillaPass::Initialize(width, height);
	}
	void Pipeline::Execute()
	{
		ShadowPass::Execute();
		VanillaPass::Execute();
	}
	void Pipeline::ResizeBuffers(unsigned width, unsigned height)
	{
		//ShadowPass::Resize(width, height);
		VanillaPass::Resize(width, height);
	}
}