#pragma once

#include <string>

#include "RHIDescriptors.h"
#include "MaterialParameter.h"
#include "Resource.h"

namespace Rhygine
{
	// TODO: In Phase 7, this class will be extended to include shader reflection data and other GPU-related information
	class MaterialDefinition : public Resource
	{
	public:
		void LoadCPU(const std::string& t_path) override;
		bool NeedsGPUUpload() const override { return false; }
		void Unload(SceneRenderer& t_sceneRenderer) override;

		const std::string& GetVertexShaderPath() const { return m_vertexShaderPath; }
		const std::string& GetPixelShaderPath() const { return m_pixelShaderPath; }
		const MaterialParameters& GetDefaults() const { return m_defaults; }
		
		// Blend / depth / rasterizer overrides (if omitted, sensible defaults are used)
		const BlendState& GetBlendState() const { return m_blendState; }
		const DepthStencilState& GetDepthStencilState() const { return m_depthStencilState; }
		const RasterizerState& GetRasterizerState() const { return m_rasterizerState; }
		bool IsTransparent() const { return m_blendState.enable; }

		// Programmatic setters
		void SetVertexShaderPath(const std::string& t_path) { m_vertexShaderPath = t_path; }
		void SetPixelShaderPath(const std::string& t_path) { m_pixelShaderPath = t_path; }
		void SetDefaults(const MaterialParameters& t_defaults) { m_defaults = t_defaults; }
		void SetBlendState(const BlendState& t_blendState) { m_blendState = t_blendState; }
		void SetDepthStencilState(const DepthStencilState& t_depthStencilState) { m_depthStencilState = t_depthStencilState; }
		void SetRasterizerState(const RasterizerState& t_rasterizerState) { m_rasterizerState = t_rasterizerState; }

	private:
		std::string m_vertexShaderPath;
		std::string m_pixelShaderPath;
		MaterialParameters m_defaults;
		BlendState m_blendState;
		DepthStencilState m_depthStencilState;
		RasterizerState m_rasterizerState;
	};
}
