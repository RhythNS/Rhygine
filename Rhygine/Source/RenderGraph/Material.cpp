#include "Material.h"

namespace Rhygine
{
	Material::Material(
		uint32_t t_id,
		Pipeline* t_pipeline,
		ResourceLayout* t_layout,
		ResourceSet* t_resourceSet,
		const MaterialDefinition* t_definition)
		: m_id(t_id)
		, m_pipeline(t_pipeline)
		, m_layout(t_layout)
		, m_resourceSet(t_resourceSet)
		, m_definition(t_definition)
	{
	}

	Material::~Material() = default;

	void Material::SetFloat(const std::string& /*t_name*/, float /*t_value*/)
	{
		// TODO: Runtime parameter updates will be connected to ResourceSet in Phase 7
	}

	void Material::SetVector(const std::string& /*t_name*/, const glm::vec4& /*t_value*/)
	{
		// TODO: Runtime parameter updates will be connected to ResourceSet in Phase 7
	}

	void Material::SetTexture(const std::string& /*t_name*/, Texture* /*t_texture*/)
	{
		// TODO: Runtime parameter updates will be connected to ResourceSet in Phase 7
	}
}
