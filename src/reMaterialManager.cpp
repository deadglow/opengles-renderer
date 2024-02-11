#include "reMaterialManager.hpp"
#include "SDL/SDL.h"

const char* DEFAULT_SHADER = "default";

using namespace reGraphics;

reMaterialManager::reMaterialManager()
	: m_defaultMaterial(reGuid<reMaterial>(0), DEFAULT_SHADER)
{
}

reMaterialManager::~reMaterialManager()
{
	m_materials.clear();
}

const reMaterial* reMaterialManager::GetMaterial(const reGuid<reMaterial> guid) const
{
	if (MaterialExists(guid))
		return &m_materials.at(guid);

	return nullptr;
}

const reMaterial& reGraphics::reMaterialManager::GetDefaultMaterial() const
{
	return m_defaultMaterial;
}

reMaterial& reGraphics::reMaterialManager::CreateMaterial(const char* shader)
{
	auto guid = reGuid<reMaterial>::CreateGUID();
	m_materials.emplace(guid, reMaterial(guid, shader));

	return m_materials[guid];
}

void reGraphics::reMaterialManager::DeleteMaterial(const reGuid<reMaterial> guid)
{
	SDL_assert(MaterialExists(guid));
	m_materials.erase(guid);
}

bool reMaterialManager::MaterialExists(const reGuid<reMaterial> guid) const
{
	return m_materials.find(guid) != m_materials.end();
}
