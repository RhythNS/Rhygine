#pragma once

#include "DataTypes/Math.h"
#include "RenderPacket.h"
#include "MaterialCache.h"

namespace Rhygine
{
    class RenderPacketBuilder
    {
    public:
        RenderPacketBuilder(MaterialCache& t_materialCache);

        void SetCamera(
            const glm::mat4& t_view,
            const glm::mat4& t_projection,
            const glm::vec3& t_position,
            float t_nearPlane,
            float t_farPlane
        );

        //const Frustum& GetFrustum() const { return m_frustum; }
        const glm::vec3& GetCameraPosition() const { return m_cameraPosition; }

        MaterialCache& GetMaterialCache() { return m_materialCache; }

        void AddRenderable(const RenderableData& t_renderable);

        void AddLight(const LightData& t_light);

        RenderPacket Build(uint32_t t_frameIndex);

    private:
        MaterialCache& m_materialCache;

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;
        glm::vec3 m_cameraPosition;
        float m_nearPlane = 0.1f;
        float m_farPlane = 1000.0f;
        //Frustum m_frustum;

        RenderPacket m_packet;
    };
}
