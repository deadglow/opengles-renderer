#pragma once

#include <unordered_map>
#include "reGuid.hpp"
#include "reTransform.hpp"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

class reHierarchy
{

private:
	umap<reGuid<reTransform>, reTransform> m_transforms;
};