#include "StackTrace.h"

#include <backward.hpp>

Rhygine::StackTrace::StackTrace(size_t t_depth, size_t t_skip_firsts)
{
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
