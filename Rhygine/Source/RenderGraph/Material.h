#pragma once

#include <cstdint>

#include "IDevice.h"
#include "MaterialDefinition.h"

namespace Rhygine
{
    class Material
    {
    public:
        Material(uint32_t t_id, Pipeline* t_pipeline, ResourceLayout* t_layout,
            ResourceSet* t_resourceSet, const MaterialDefinition* t_definition);
        ~Material();
        
        // --- Accessors used by the draw loop and extractors ---
        uint32_t GetId() const { return m_id; }
        Pipeline* GetPipeline() const { return m_pipeline; }
        ResourceSet* GetResourceSet() const { return m_resourceSet; }
        
        // Back-reference to the definition (for debug display, hot-reload lookup)
        const MaterialDefinition* GetDefinition() const { return m_definition; }
        bool IsTransparent() const { return m_definition->IsTransparent(); }
        
        // --- Parameter updates at runtime ---
        // Allows tweaking material parameters without creating a new Material
        void SetFloat(const std::string& t_name, float t_value);
        void SetVector(const std::string& t_name, const glm::vec4& t_value);
        void SetTexture(const std::string& t_name, Texture* t_texture);

    private:
        uint32_t m_id; // Stable ID for sort keys
        Pipeline* m_pipeline; // Compiled PSO (shaders + render state)
        ResourceLayout* m_layout; // Descriptor layout
        ResourceSet* m_resourceSet; // Bound descriptor set (textures + buffers)
        const MaterialDefinition* m_definition; // Source definition
    };
}
