#include "MaterialDefinition.h"

namespace Rhygine
{
	void MaterialDefinition::LoadCPU(const std::string& /*t_path*/)
	{
		// Asset file parsing will be hooked into the asset loader pipeline.
		m_state = State::CPUReady;
	}

	void MaterialDefinition::Unload(SceneRenderer& t_sceneRenderer)
	{
		m_vertexShaderPath.clear();
		m_pixelShaderPath.clear();
		m_defaults.parameters.clear();
		m_blendState = {};
		m_depthStencilState = {};
		m_rasterizerState = {};
		m_state = State::Unloaded;
	}
}
