#pragma once

#include "CommonTypes.hpp"
#include <unordered_map>

namespace fastPRNG
{
	class fastXS64;
};

namespace Enoki
{
	struct GuidGeneration
	{
		static uint64_t CreateU64();
		static uint64_t CreateU64FromSeed(const uint64_t seed);
		static fastPRNG::fastXS64& GetRandGenerator();
	};


	template <class T>
	struct Guid
	{
	public:
		Guid() = default;
		Guid(const uint64 value) : value(value) {}

		bool operator==(const Guid<T>& other) const
		{
			return value == other.value;
		}

		bool IsValid() const { return value > 0; }

		uint64 value;

	public:
		static Guid<T> CreateGUID()
		{
			return Guid<T>(GuidGeneration::CreateU64());
		}

		static Guid<T> CreateGUIDFromSeed(const uint64 seed)
		{
			return Guid<T>(GuidGeneration::CreateU64FromSeed(seed));
		}
	};
}


template<>
template <typename T>
struct std::hash<Enoki::Guid<T>>
{
	std::size_t operator()(const Enoki::Guid<T>& k) const
	{
		return static_cast<std::size_t>(k.value);
	}
};

template<>
template<typename T>
struct std::equal_to<Enoki::Guid<T>>
{
	bool operator()(const Enoki::Guid<T>& lhs, const Enoki::Guid<T>& rhs) const
	{
		return lhs == rhs;
	}
};