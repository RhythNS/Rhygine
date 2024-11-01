#pragma once
#include <string>

namespace Rhygine
{
	class StackTrace
	{
	public:
		StackTrace(size_t t_depth = 32, size_t t_skip_firsts = 1);
		[[nodiscard]] const std::string& Print() const;

	private:
		std::string buffer;
	};
}
