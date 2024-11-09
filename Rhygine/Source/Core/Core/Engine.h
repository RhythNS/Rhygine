#pragma once

#include <memory>

#include "Gfx.h"

namespace Rhygine
{
	class Config;

	class Engine
	{
	public:
		Engine() = delete;
		Engine(Config* t_config);

		int Run();

	private:
		inline void SetupGfx();

	private:
		Config* m_config;
		std::unique_ptr<Gfx> m_gfx = nullptr;
	};
}
