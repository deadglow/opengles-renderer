#pragma once
#include <string_view>

namespace Enoki::Graphics
{
	class Model;

	namespace ModelLoader
	{
		bool LoadFromFile(std::string_view filename, Model& model);
	};
};