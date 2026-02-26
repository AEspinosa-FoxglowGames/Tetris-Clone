#pragma once
#include <random>
#include <cstdint>

namespace Random
{
	void Init(uint32_t seed = 0);

	int Int(int minInclusive, int maxInclusive);
	float Float(float minInclusive, float maxInclusive);

	std::mt19937& Engine();
}