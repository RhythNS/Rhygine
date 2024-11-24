#pragma once

#include "FileProvider.h"

#include <string>

namespace Rhygine
{
	class StandardFileProvider : public FileProvider
	{
	public:
		StandardFileProvider() = delete;
		StandardFileProvider(const std::string& t_name, const std::string& t_path);
		[[nodiscard]] bool Has(const std::string& t_path) const override;
		[[nodiscard]] std::unique_ptr<File> Open(const std::string& t_path, FileMode t_fileMode) const override;
	
	private:
		const std::string m_path;
	};
}
