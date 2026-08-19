#include "System.h"

#include <string>
#include <tracy/Tracy.hpp>

#include "Debug/Logger.h"
#include "Debug/Error.h"
#include "File/StandardFileProvider.h"

Rhygine::System* Rhygine::System::s_instance = nullptr;

Rhygine::System::System(Config& t_config)
{
	ZoneScoped;
	ASSERT_ERROR_MESSAGE(s_instance == nullptr, "System is already set!");

	s_instance = this;

	// TODO:
	m_fileManager = std::make_unique<FileManager>();
	m_fileManager->Mount(std::make_unique<StandardFileProvider>("res", "res/"));
	LOG_INFO("System: Base subsystem initialized");
}

Rhygine::System::~System()
{
	ZoneScoped;
	LOG_INFO("System: Base subsystem shutting down");
	s_instance = nullptr;
}

Rhygine::System* Rhygine::System::GetInstance()
{
	return s_instance;
}

Rhygine::FileManager& Rhygine::System::GetFileManager() const
{
	return *m_fileManager;
}
