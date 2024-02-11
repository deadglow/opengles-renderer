#include "reGuid.hpp"
#include "fastPRNG.h"

#ifdef DEBUG_RANDOM
fastPRNG::fastXS64 s_rand = fastPRNG::fastXS64(0);
#else
fastPRNG::fastXS64 s_rand = fastPRNG::fastXS64();
#endif

uint64_t reGuidGeneration::CreateU64()
{
	return (uint64_t)s_rand.xoroshiro128p();
}

uint64_t reGuidGeneration::CreateU64FromSeed(const uint64_t seed)
{
	fastPRNG::fastXS64 rand(seed);
	return (uint64_t)rand.xoroshiro128p();
}

fastPRNG::fastXS64& reGuidGeneration::GetRandGenerator()
{
	return s_rand;
}
