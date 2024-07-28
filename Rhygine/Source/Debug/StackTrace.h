#pragma once
#include <string>

namespace Rhygine
{
	class StackTrace
	{
	public:
		StackTrace();
		const std::string& Print() const;

	private:
		std::string buffer;
	};
}
