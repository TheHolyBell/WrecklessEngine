#include "Noise.h"

#include "FastNoise.h"

namespace Misc
{
	float Noise::PerlinNoise(float x, float y)
	{
		static FastNoise _FastNoise;
		_FastNoise.SetNoiseType(FastNoise::Perlin);
		return _FastNoise.GetNoise(x, y);
	}
}