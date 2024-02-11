#pragma once

#include <stdint.h>
#include <unordered_map>

namespace fastPRNG
{
	class fastXS64;
};

struct reGuidGeneration
{
	static uint64_t CreateU64();
	static uint64_t CreateU64FromSeed(const uint64_t seed);
	static fastPRNG::fastXS64& GetRandGenerator();
};


template <class T>
struct reGuid
{
public:
	reGuid() = default;
	reGuid(const uint64_t value) : m_value(value) {}

	bool operator==(const reGuid<T>& other) const
	{
		return m_value == other.m_value;
	}

	bool IsValid() const { return m_value > 0; }

	uint64_t m_value;

public:
	static reGuid<T> CreateGUID()
	{
		return reGuid<T>(reGuidGeneration::CreateU64());
	}

	static reGuid<T> CreateGUIDFromSeed(const uint64_t seed)
	{
		return reGuid<T>(reGuidGeneration::CreateU64FromSeed(seed));
	}
};


template<>
template <typename T>
struct std::hash<reGuid<T>>
{
	std::size_t operator()(const reGuid<T>& k) const
	{
		return (std::size_t)k.m_value;
	}
};

template<>
template<typename T>
struct std::equal_to<reGuid<T>>
{
	std::size_t operator()(const reGuid<T>& lhs, const reGuid<T>& rhs) const
	{
		return lhs == rhs;
	}
};