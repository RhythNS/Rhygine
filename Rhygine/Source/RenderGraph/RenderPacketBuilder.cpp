#include "RenderPacketBuilder.h"

#include <utility>

namespace Rhygine
{
	RenderPacketBuilder::RenderPacketBuilder(MaterialCache& t_materialCache)
		: m_materialCache(t_materialCache)
		, m_viewMatrix(1.0f)
		, m_projectionMatrix(1.0f)
		, m_cameraPosition(0.0f)
		, m_nearPlane(0.1f)
		, m_farPlane(1000.0f)
	{
	}

	void RenderPacketBuilder::SetCamera(
		const glm::mat4& t_view,
		const glm::mat4& t_projection,
		const glm::vec3& t_position,
		float t_nearPlane,
		float t_farPlane)
	{
		m_viewMatrix = t_view;
		m_projectionMatrix = t_projection;
		m_cameraPosition = t_position;
		m_nearPlane = t_nearPlane;
		m_farPlane = t_farPlane;
		// m_frustum = Frustum(t_projection * t_view); // TODO: Follow-up when Frustum class is implemented
	}

	void RenderPacketBuilder::AddRenderable(const RenderableData& t_renderable)
	{
		m_packet.renderables.push_back(t_renderable);
	}

	void RenderPacketBuilder::AddLight(const LightData& t_light)
	{
		m_packet.lights.push_back(t_light);
	}

	RenderPacket RenderPacketBuilder::Build(uint32_t t_frameIndex)
	{
		m_packet.viewMatrix = m_viewMatrix;
		m_packet.projectionMatrix = m_projectionMatrix;
		m_packet.cameraPosition = m_cameraPosition;
		m_packet.nearPlane = m_nearPlane;
		m_packet.farPlane = m_farPlane;
		m_packet.frameIndex = t_frameIndex;

		RenderPacket result = std::move(m_packet);
		m_packet = {}; // Reset for next frame
		return result;
	}
}
