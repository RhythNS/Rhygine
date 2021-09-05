#pragma once
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <comdef.h>

#define RHY_EXCEP( message ) RhyException(message, __FILE__, __LINE__)
#define THROW_IF_FAILED( method ) if (FAILED(method)) throw RhyException(__FILE__, __LINE__)

/// <summary>
/// Exceptions for engine related errors.
/// </summary>
class RhyException : public std::exception
{
public:
	RhyException(std::string message, std::string file, int line) : message(message), file(file), line(line) {}
	RhyException(std::string file, int line) : message(""), file(file), line(line) {}
	
	virtual const char* what() const throw()
	{
		_com_error error(GetLastError());
		LPCTSTR errorText = error.ErrorMessage();

		std::ostringstream str;
		str << message << std::endl <<
			std::string(errorText) << std::endl <<
			file << " : " << line << std::endl;

		whatString = str.str();

		return whatString.c_str() ;
	}

	std::string GetFile() {
		return file;
	}

	const int GetLine() {
		return line;
	}
private:
	mutable std::string whatString;

	std::string message;
	std::string file;
	int line;
};
