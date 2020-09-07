#include "NoiseCSharp.h"
#include "ScriptingEngine.h"
#include "Noise.h"

namespace Scripting
{
	void NoiseCSharp::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Math.Noise::PerlinNoise_Native", Misc::Noise::PerlinNoise);
	}
}