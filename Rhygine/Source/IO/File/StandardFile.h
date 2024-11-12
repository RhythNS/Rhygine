#pragma once

#include "File.h"
#include <fstream>

namespace Rhygine
{
	class StandardFile : public File
	{
	public:
		StandardFile() = delete;
		StandardFile(std::string& t_filePath, FileMode t_mode);
		~StandardFile();

		virtual size_t Read(void* buffer, size_t size) override;
		virtual std::istream& GetInputStream() override;

		virtual size_t Write(const void* buffer, size_t size) override;
		virtual std::ostream& GetOutputStream() override;

		virtual void Seek(long offset, SeekOrigin origin) override;

		virtual size_t Size() override;

		virtual bool Good() const override;
	
	private:
		std::fstream m_fileStream;
	};
}
