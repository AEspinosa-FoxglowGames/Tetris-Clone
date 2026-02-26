#include "random.h"

namespace Random
{
	static std::mt19937 g_engine;

	void Init(uint32_t seed)
	{
		if (seed == 0)
		{
			std::random_device rd;
			std::seed_seq seq{ rd(),rd(),rd(),rd(), static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(&g_engine)) };
			g_engine.seed(seq);
		}
		else
		{
			g_engine.seed(seed);
		}
	}
	std::mt19937& Engine()
	{
		return g_engine;
	}
	int Int(int minInclusive, int maxInclusive)
	{
		std::uniform_int_distribution<int>dist(minInclusive, maxInclusive);
		return dist(g_engine);
	}
	float Float(float minInclusive, float maxInclusive)
	{
		std::uniform_real_distribution<float>dist(minInclusive, maxInclusive);
		return dist(g_engine);
	}
}