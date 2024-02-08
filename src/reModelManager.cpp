#include "reModelManager.hpp"
#include "SDL/SDL.h"

using namespace reGraphics;

reGraphics::reModelManager::reModelManager()
{
	// default shapes
	LoadNewPrimitive("default_quad", rePrimitives::Quad);
}

reGraphics::reModelManager::~reModelManager()
{
	UnloadAllModels();
}

void reGraphics::reModelManager::UnloadAllModels()
{
	for (auto it = m_models.cbegin(); it != m_models.cend();)
	{
		const reModel* model = &(it->second);
		if (model->m_loadedOnGpu)
		{
			GPUUnload(model->m_name);
		}

		++it;
	}

	m_models.clear();
}

reModel* reGraphics::reModelManager::CreateModel(const std::string& name)
{
	SDL_assert(!IsModelLoaded(name));

	m_models.emplace(name, reModel(name));

	return &m_models[name];
}

const reModel* reGraphics::reModelManager::LoadModel(const std::string& filename)
{
	SDL_assert(!IsModelLoaded(filename));

	const reModel* model = CreateModel(filename);

	return nullptr;
}

void reGraphics::reModelManager::UnloadModel(const std::string& name)
{
	SDL_assert(IsModelLoaded(name));

	const reModel* model = &m_models[name];
	if (model->m_loadedOnGpu)
	{
		GPUUnload(name);
	}
	model = nullptr;

	m_models.erase(name);
}

const reModel* reGraphics::reModelManager::LoadNewPrimitive(const std::string& name, rePrimitives::PrimitiveType primitiveType)
{
	reModel* model = CreateModel(name);
	rePrimitives::ConstructPrimitive(primitiveType, *model);

	return model;
}

const reModel* reGraphics::reModelManager::GetModel(const std::string& name) const
{
	if (IsModelLoaded(name))
		return &m_models.at(name);

	return nullptr;
}

bool reGraphics::reModelManager::IsModelLoaded(const std::string& name) const
{
	return m_models.find(name) != m_models.end();
}

void reGraphics::reModelManager::GPULoad(const std::string& name)
{
	SDL_assert(IsModelLoaded(name));
	reModel* model = &m_models[name];

	// Todo

	model->m_loadedOnGpu = true;
}

void reGraphics::reModelManager::GPUUnload(const std::string& name)
{
	SDL_assert(IsModelLoaded(name));
	reModel* model = &m_models[name];

	//todo
	model->m_loadedOnGpu = false;
}
