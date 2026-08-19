#include "StackTrace.h"

#include <backward.hpp>
#include <tracy/Tracy.hpp>

Rhygine::StackTrace::StackTrace(size_t t_depth, size_t t_skip_firsts)
{
	ZoneScoped;
	backward::StackTrace internalStackTrace;
	backward::Printer printer;

	internalStackTrace.skip_n_firsts(t_skip_firsts);
	internalStackTrace.load_here(t_depth);

	std::stringstream stringStream;

	printer.print(internalStackTrace, stringStream);

	buffer = stringStream.str();
}

const std::string& Rhygine::StackTrace::Print() const
{
	return buffer;
}
