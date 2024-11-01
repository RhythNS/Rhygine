#pragma once

namespace Rhygine
{
	class Config;

	class Engine
	{
	public:
		Engine() = delete;
		Engine(Config* t_config);

		int Run();
	};
}
