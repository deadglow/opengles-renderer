#pragma once

#include <vector>
#include <unordered_map>

#include "Guid.hpp"
#include "Model.hpp"
#include "PrimitiveFactory.hpp"

// loads primitives by default named:
//	"default_quad"

namespace Enoki::Graphics
{
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		const Model* GetModel(const Guid<Model> guid) const;
		const Guid<Model>* GetModelIDByName(std::string_view name) const;

		void UnloadAllModels();

		Model& CreateEmptyModel(std::string_view name);

		const Model& LoadModel(std::string_view filename);
		void UnloadModel(const Guid<Model> guid);

		const Model& LoadNewPrimitive(std::string_view name, const Primitives::PrimitiveType primitiveType);

		bool IsModelLoaded(const Guid<Model> guid) const;
		bool IsModelLoaded(std::string_view name) const;

		// GPU stuff
		void GPULoad(const Guid<Model> guid);
		void GPUUnload(const Guid<Model> guid);

	private:
		std::unordered_map<Guid<Model>, Model> m_models;
		std::unordered_map<std::string, Guid<Model>> m_guidLookup;
	};
}
