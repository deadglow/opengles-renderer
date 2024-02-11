#pragma once

#include <vector>
#include <unordered_map>

#include "reGuid.hpp"
#include "reModel.hpp"
#include "rePrimitiveFactory.hpp"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

// loads primitives by default named:
//	"default_quad"

namespace reGraphics
{
	class reModelManager
	{
	public:
		reModelManager();
		~reModelManager();

		const reModel* GetModel(const reGuid<reModel> guid) const;
		const reGuid<reModel>* GetModelIDByName(const char* name) const;

		void UnloadAllModels();

		reModel& CreateEmptyModel(const char* name);

		const reModel& LoadModel(const char* filename);
		void UnloadModel(const reGuid<reModel> guid);

		const reModel& LoadNewPrimitive(const char* name, const rePrimitives::PrimitiveType primitiveType);

		bool IsModelLoaded(const reGuid<reModel> guid) const;
		bool IsModelLoaded(const char* name) const;

		// GPU stuff
		void GPULoad(const reGuid<reModel> guid);
		void GPUUnload(const reGuid<reModel> guid);

	private:
		umap<reGuid<reModel>, reModel> m_models;
		umap<std::string, reGuid<reModel>> m_guidLookup;
	};
}
