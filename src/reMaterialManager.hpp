#pragma once

#include <unordered_map>
#include "reMaterial.hpp"
#include "reGuid.hpp"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

namespace reGraphics
{
	class reMaterialManager
	{
	public:
		reMaterialManager();
		~reMaterialManager();

		bool MaterialExists(const reGuid<reMaterial> guid) const;
		const reMaterial* GetMaterial(const reGuid<reMaterial> guid) const;
		const reMaterial& GetDefaultMaterial() const;
		const reMaterial& GetLinesMaterial() const;

		reMaterial& CreateMaterial(const char* shader);

		void DeleteMaterial(const reGuid<reMaterial> guid);

	private:
		umap<reGuid<reMaterial>, reMaterial> m_materials;
		reMaterial m_defaultMaterial;
		reMaterial m_linesMaterial;
	};
}
