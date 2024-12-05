#pragma once

#include <vector>
#include <string>
#include "Guid.hpp"
#include "Uniform.hpp"

namespace Enoki::Graphics
{
	class ShaderManager;

	class Material
	{
	public:
		Material() = default;
		Material(const Guid<Material> guid, std::string_view shaderName)
			: guid(guid)
			, m_shaderName(shaderName)
		{}

		void AddUniform(const char* name, const UniformValue& value);

		void UseMaterial() const;
		void ApplyUniform(const char* uniformName, const UniformValue& uniformValue) const;

		Guid<Material> guid;
		std::string m_shaderName;

		std::string name;
		std::vector<Uniform> m_shaderUniforms;
	};
};