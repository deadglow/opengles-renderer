#pragma once

#include <vector>
#include <string>
#include "reGuid.hpp"
#include "reUniform.hpp"

namespace reGraphics
{
	class reShaderManager;

	class reMaterial
	{
	public:
		reMaterial() = default;
		reMaterial(const reGuid<reMaterial> guid, const char* shaderName)
			: m_guid(guid)
			, m_shaderName(shaderName)
		{}

		void AddUniform(const char* name, const reUniformValue& value);

		void UseMaterial() const;
		void ApplyUniform(const char* uniformName, const reUniformValue& uniformValue) const;

		reGuid<reMaterial> m_guid;
		std::string m_shaderName;

		std::string m_name;
		std::vector<reUniform> m_shaderUniforms;
	};
};