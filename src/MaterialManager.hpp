#pragma once

#include <unordered_map>
#include "Material.hpp"
#include "Guid.hpp"

namespace Enoki::Graphics
{
	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

		bool MaterialExists(const Guid<Material> guid) const;
		const Material* GetMaterial(const Guid<Material> guid) const;
		const Material& GetDefaultMaterial() const;
		const Material& GetLinesMaterial() const;

		Material& CreateMaterial(std::string_view shader);

		void DeleteMaterial(const Guid<Material> guid);

	private:
		std::unordered_map<Guid<Material>, Material> materials;
		Material m_defaultMaterial;
		Material m_linesMaterial;
	};
}
