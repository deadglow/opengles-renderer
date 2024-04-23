#pragma once

namespace reGraphics
{
	class reModel;

	namespace reModelLoader
	{
		bool LoadFromFile(const char* filename, reModel& model);
	};
};