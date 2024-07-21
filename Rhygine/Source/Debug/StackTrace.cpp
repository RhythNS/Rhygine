#include "StackTrace.h"

#include <backward.hpp>

Rhygine::StackTrace::StackTrace()
{
	backward::StackTrace internalStackTrace;
	backward::Printer printer;

	internalStackTrace.skip_n_firsts(1);
	internalStackTrace.load_here(32);

	std::stringstream stringStream;

	printer.print(internalStackTrace, stringStream);

	buffer = stringStream.str();
}

std::string& Rhygine::StackTrace::Print() const
{
	return buffer;
}
