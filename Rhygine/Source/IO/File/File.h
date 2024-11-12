#pragma once

#include <string>
#include <vector>

namespace Rhygine
{
	enum FileMode { Read = 1, Write = 2, Append = 4, Trunc = 8, Binary = 16};
	enum class SeekOrigin { Begin, Current, End };

	class File
	{
	public:
		File() = delete;
		File(std::string& t_filePath, FileMode t_mode);
		virtual ~File() = default;

		virtual size_t Read(void* buffer, size_t size) = 0;
		virtual std::vector<char> ReadAll();
        virtual std::istream& GetInputStream() = 0;

        virtual size_t Write(const void* buffer, size_t size) = 0;
		virtual void WriteAllBytes(const std::vector<char>& data);
        virtual std::ostream& GetOutputStream() = 0;

        virtual void Seek(long offset, SeekOrigin origin) = 0;

        virtual size_t Size() = 0;

        virtual bool Good() const = 0;
	
	protected:
		std::string m_filePath;
		FileMode m_fileMode;
	};
}
