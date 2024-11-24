#include "StandardFileProvider.h"

#include <filesystem>

#include "StandardFile.h"
#include "Debug\Error.h"

Rhygine::StandardFileProvider::StandardFileProvider(const std::string& t_name, const std::string& t_path)
	: FileProvider(t_name), m_path(t_path)
{
}

bool Rhygine::StandardFileProvider::Has(const std::string& t_path) const
{
	return std::filesystem::exists(m_path + t_path);
}

std::unique_ptr<Rhygine::File> Rhygine::StandardFileProvider::Open(const std::string& t_path, FileMode t_fileMode) const
{
	if (!Has(t_path))
	{
		LOG_ERROR("Could not find file: " + m_path + t_path);
		return nullptr;
	}
	std::string path = m_path + t_path;
	return std::make_unique<StandardFile>(path, t_fileMode);
}
