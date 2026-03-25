#pragma once

#ifdef _WIN32
#include "Windows/RhyWindows.h"
#endif // _WIN32
#include "Config/Config.h"

namespace Rhygine
{
	class Config;

	class EntryPoint
	{
	public:
		EntryPoint() = delete;

		static int Run(int t_args_count, char* t_args[]);

#ifdef _WIN32
		static int WinRun(HINSTANCE t_instance, HINSTANCE t_prev_instance, LPSTR t_args, int t_arg_count);
#endif // _WIN32

	private:
		static inline void ParseRuntimeArguments(std::vector<std::string>& t_args, Config& t_config);
	};
}
