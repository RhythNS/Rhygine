#include "FileProvider.h"

Rhygine::FileProvider::FileProvider(const std::string& t_name) : m_name(t_name)
{

}

std::string Rhygine::FileProvider::GetName() const
{
	return m_name;
}
