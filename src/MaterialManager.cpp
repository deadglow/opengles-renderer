#include "MaterialManager.hpp"
#include "SDL/SDL.h"

const char* DEFAULT_SHADER = "default";
const char* LINES_SHADER = "lines";

using namespace Enoki::Graphics;

MaterialManager::MaterialManager()
	: m_defaultMaterial(Guid<Material>(0), DEFAULT_SHADER)
	, m_linesMaterial(Guid<Material>(1), LINES_SHADER)
{
}

MaterialManager::~MaterialManager()
{
	materials.clear();
}

const Material* MaterialManager::GetMaterial(const Guid<Material> guid) const
{
	if (MaterialExists(guid))
		return &materials.at(guid);

	return nullptr;
}

const Material& MaterialManager::GetDefaultMaterial() const
{
	return m_defaultMaterial;
}

const Material& MaterialManager::GetLinesMaterial() const
{
	return m_linesMaterial;
}

Material& MaterialManager::CreateMaterial(std::string_view shader)
{
	auto guid = Guid<Material>::CreateGUID();
	materials.emplace(guid, Material(guid, shader));

	return materials[guid];
}

void MaterialManager::DeleteMaterial(const Guid<Material> guid)
{
	SDL_assert(MaterialExists(guid));
	materials.erase(guid);
}

bool MaterialManager::MaterialExists(const Guid<Material> guid) const
{
	return materials.find(guid) != materials.end();
}
