#pragma once

#include <string>
#include <vector>
#include <memory>

#include "FileProvider.h"
#include "DataTypes\Concurrency\SharedMutex.h"

namespace Rhygine
{
	class FileManager
	{
	public:
		void Mount(std::unique_ptr<FileProvider> t_provider);
		
		[[nodiscard]] std::unique_ptr<File> Open(const std::string& t_path, FileMode t_fileMode) const;
		[[nodiscard]] std::unique_ptr<File> Open(const std::string& t_provider, const std::string& t_path, FileMode t_fileMode) const;
		[[nodiscard]] bool Has(const std::string& t_path) const;
		[[nodiscard]] bool Has(const std::string& t_provider, const std::string& t_path) const;

	private:
		std::vector<std::unique_ptr<FileProvider>> m_providers;
		mutable SharedMutex m_sharedMutex;
	};
}
