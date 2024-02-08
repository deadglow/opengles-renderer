#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "reModel.hpp"
#include "rePrimitiveFactory.hpp"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

namespace reGraphics
{
	class reModelManager
	{
	public:
		reModelManager();
		~reModelManager();

		void UnloadAllModels();

		reModel* CreateModel(const std::string& name);

		const reModel* LoadModel(const std::string& filename);
		void UnloadModel(const std::string& name);

		const reModel* LoadNewPrimitive(const std::string& name, rePrimitives::PrimitiveType primitiveType);

		const reModel* GetModel(const std::string& name) const;

		bool IsModelLoaded(const std::string& name) const;

		// GPU stuff
		void GPULoad(const std::string& name);
		void GPUUnload(const std::string& name);

	private:
		umap<std::string, reModel> m_models;
	};
}
