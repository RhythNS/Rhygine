#pragma once

#include <string>
#include <memory>

#include "File.h"

namespace Rhygine
{
	class FileProvider
	{
	public:
		FileProvider() = delete;
		FileProvider(const std::string& t_name);
		[[nodiscard]] std::string GetName() const;

		virtual bool Has(const std::string& t_path) const = 0;
		[[nodiscard]] virtual std::unique_ptr<File> Open(const std::string& t_path, FileMode t_fileMode) const = 0;

	private:
		const std::string m_name;
	};
}
