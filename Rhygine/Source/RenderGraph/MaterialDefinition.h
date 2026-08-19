#pragma once

#include <expected>
#include <string>

#include "RHIDescriptors.h"
#include "MaterialParameter.h"
#include "Resource.h"
#include "StructuredIO/Describe.h"
#include "StructuredIO/IOError.h"

namespace Rhygine
{
	// TODO: shader reflection data and other GPU-related information
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

	template <>
	struct Describe<MaterialDefinition>
	{
		static void Serialize(const MaterialDefinition& t_obj, PropertyMap& t_map)
		{
			t_map.Set("vertex_shader", t_obj.GetVertexShaderPath());
			t_map.Set("pixel_shader", t_obj.GetPixelShaderPath());

			PropertyMap blend;
			blend.Set("enable", t_obj.GetBlendState().enable);
			t_map.Set("blend", std::move(blend));

			PropertyMap depth;
			depth.Set("enable", t_obj.GetDepthStencilState().depthEnable);
			depth.Set("write", t_obj.GetDepthStencilState().depthWriteEnable);
			t_map.Set("depth", std::move(depth));

			PropertyMap defaults;
			for (const MaterialParameter& param : t_obj.GetDefaults().parameters)
			{
				switch (param.type)
				{
				case MaterialParameter::Type::Float:
					defaults.Set(param.name, static_cast<double>(param.floatValue));
					break;
				case MaterialParameter::Type::Vec2:
					defaults.Set(param.name, glm::vec2(param.vec2Value));
					break;
				case MaterialParameter::Type::Vec3:
					defaults.Set(param.name, glm::vec3(param.vec3Value));
					break;
				case MaterialParameter::Type::Vec4:
					defaults.Set(param.name, glm::vec4(param.vec4Value));
					break;
				case MaterialParameter::Type::Texture:
					defaults.Set(param.name, param.texturePath);
					break;
				}
			}
			t_map.Set("defaults", std::move(defaults));
		}

		static std::expected<void, IOError> Deserialize(const PropertyMap& t_map, MaterialDefinition& t_obj)
		{
			auto vs = t_map.TryGet<std::string>("vertex_shader");
			if (!vs) return std::unexpected(IOError::MissingKey("vertex_shader"));
			t_obj.SetVertexShaderPath(*vs);

			auto ps = t_map.TryGet<std::string>("pixel_shader");
			if (!ps) return std::unexpected(IOError::MissingKey("pixel_shader"));
			t_obj.SetPixelShaderPath(*ps);

			if (const PropertyMap* blend = t_map.TryGetSubMap("blend"))
			{
				BlendState bs{};
				bs.enable = blend->GetOr<bool>("enable", false);
				t_obj.SetBlendState(bs);
			}

			if (const PropertyMap* depth = t_map.TryGetSubMap("depth"))
			{
				DepthStencilState ds{};
				ds.depthEnable = depth->GetOr<bool>("enable", true);
				ds.depthWriteEnable = depth->GetOr<bool>("write", true);
				t_obj.SetDepthStencilState(ds);
			}

			if (const PropertyMap* defaults = t_map.TryGetSubMap("defaults"))
			{
				MaterialParameters params;
				defaults->ForEach([&](const std::string& key, const PropertyMap::Value& val)
				{
					MaterialParameter param;
					param.name = key;
					if (std::holds_alternative<double>(val))
					{
						param.type = MaterialParameter::Type::Float;
						param.floatValue = static_cast<float>(std::get<double>(val));
						params.parameters.push_back(param);
					}
					else if (std::holds_alternative<int64_t>(val))
					{
						param.type = MaterialParameter::Type::Float;
						param.floatValue = static_cast<float>(std::get<int64_t>(val));
						params.parameters.push_back(param);
					}
					else if (std::holds_alternative<glm::vec2>(val))
					{
						param.type = MaterialParameter::Type::Vec2;
						param.vec2Value = std::get<glm::vec2>(val);
						params.parameters.push_back(param);
					}
					else if (std::holds_alternative<glm::vec3>(val))
					{
						param.type = MaterialParameter::Type::Vec3;
						param.vec3Value = std::get<glm::vec3>(val);
						params.parameters.push_back(param);
					}
					else if (std::holds_alternative<glm::vec4>(val))
					{
						param.type = MaterialParameter::Type::Vec4;
						param.vec4Value = std::get<glm::vec4>(val);
						params.parameters.push_back(param);
					}
					else if (std::holds_alternative<std::string>(val))
					{
						param.type = MaterialParameter::Type::Texture;
						param.texturePath = std::get<std::string>(val);
						params.parameters.push_back(param);
					}
				});
				t_obj.SetDefaults(params);
			}

			return {};
		}
	};
}
