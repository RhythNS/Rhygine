#pragma once

#include <string>

#include "RHIDescriptors.h"
#include "MaterialParameter.h"
#include "Resource.h"

namespace Rhygine
{
	class MaterialDefinition : public Resource
	{
	public:
		void LoadCPU(const std::string& t_path) override;
		bool NeedsGPUUpload() const override { return false; }
		void Unload() override;

		const std::string& GetVertexShaderPath() const { return m_vertexShaderPath; }
		const std::string& GetPixelShaderPath() const { return m_pixelShaderPath; }
		const MaterialParameters& GetDefaults() const { return m_defaults; }
		
		// Blend / depth / rasterizer overrides (if omitted, sensible defaults are used)
		const BlendState& GetBlendState() const { return m_blendState; }
		const DepthStencilState& GetDepthStencilState() const { return m_depthStencilState; }
		const RasterizerState& GetRasterizerState() const { return m_rasterizerState; }
		bool IsTransparent() const { return m_blendState.enable; }

	private:
		std::string m_vertexShaderPath;
		std::string m_pixelShaderPath;
		MaterialParameters m_defaults;
		BlendState m_blendState;
		DepthStencilState m_depthStencilState;
		RasterizerState m_rasterizerState;
	};
}
