#include "TomlInterpreter.h"

#include <tracy/Tracy.hpp>
#include "File/File.h"
#include "Debug/Error.h"

bool Rhygine::TomlInterpreter::Load(File& t_file)
{
	ZoneScoped;
	try
	{
		m_table = toml::parse(t_file.GetInputStream());
		return true;
	}
	catch (const toml::parse_error& err)
	{
		LOG_ERROR("Parsing failed:" + std::string(err.what()));
		return false;
	}
}

void Rhygine::TomlInterpreter::Save(File& t_file)
{
	ZoneScoped;
	t_file.GetOutputStream() << m_table;
}
