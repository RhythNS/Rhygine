#include "MaterialDefinition.h"

#include <tracy/Tracy.hpp>
#include "System.h"
#include "File/FileManager.h"
#include "StructuredIO/Read.h"
#include "Debug/Logger.h"

namespace Rhygine
{
    void MaterialDefinition::LoadCPU(const std::string& t_path)
    {
        ZoneScoped;
        auto file = System::GetInstance()->GetFileManager().Open(t_path, FileMode::Read);
        if (!file)
        {
            LOG_ERROR("Failed to open: " + t_path);
            m_state = State::Failed;
            return;
        }

        auto result = IO::Read<MaterialDefinition>(*file, *this);
        if (!result)
        {
            LOG_ERROR("Failed to read MaterialDefinition: " + result.error().message);
            m_state = State::Failed;
            return;
        }

        LOG_INFO("Successfully loaded MaterialDefinition: " + t_path);
        m_state = State::CPUReady;
    }

    void MaterialDefinition::Unload(SceneRenderer& t_sceneRenderer)
    {
        ZoneScoped;
        m_vertexShaderPath.clear();
        m_pixelShaderPath.clear();
        m_defaults.parameters.clear();
        m_blendState = {};
        m_depthStencilState = {};
        m_rasterizerState = {};
        m_state = State::Unloaded;
    }
}
