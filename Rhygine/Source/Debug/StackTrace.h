#include <string>

namespace Rhygine
{
	class StackTrace
	{
	public:
		StackTrace();
		std::string& Print() const;

	private:
		mutable std::string buffer;
	};
}
