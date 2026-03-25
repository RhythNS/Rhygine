#include "Engine.h"

#include <tracy/Tracy.hpp>

#include "Config/Config.h"
#include "System.h"
#include "Debug/Logger.h"
#include "Debug/Error.h"
#ifdef _WIN32
#include "DX11/DX11.h"
#endif

Rhygine::Engine::Engine(Config* t_config) : m_config(t_config)
{
	ZoneScoped;

	SetupGfx();

	/*
	if (t_config.TryGet<bool>("System/AttachConsole").value_or(false))
	{

	}
	*/

	System* system = System::GetInstance();
	system->CreateConsole();
	system->AddWindow();


	LOG_INFO("Rhygine initialized");
}

inline void Rhygine::Engine::SetupGfx()
{
	// TODO: Change this
	std::string backend = m_config->GetOr<std::string>("Gfx/Backend", "DX11");

	if (backend == "DX11")
	{
#ifdef _WIN32
		m_gfx.reset(new DX11());
#else
		STOP_EXECUTION_MESSAGE(backend + " is only available with windows!");
#endif
	}
	else
	{
		STOP_EXECUTION_MESSAGE(backend + " not found!");
	}
}

int Rhygine::Engine::Run()
{
	ZoneScoped;
	
	System* system = System::GetInstance();
	int exitCode = 0;
	bool running = true;

	while (running)
	{
		ZoneScopedN("Rhygine::Engine::Run::Loop");

		std::optional<int> optExitCode = system->ProcessMessages();
		if (optExitCode.has_value())
		{
			exitCode = optExitCode.value();
			running = false;
		}

		m_gfx->Draw();
		FrameMark;
	}

	return exitCode;
}
