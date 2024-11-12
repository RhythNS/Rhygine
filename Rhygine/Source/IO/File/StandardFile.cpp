#include "StandardFile.h"

#include <filesystem> 

#include "Debug/Error.h"

Rhygine::StandardFile::StandardFile(std::string& t_filePath, FileMode t_mode)
	: File(t_filePath, t_mode)
{
    ASSERT_ERROR_MESSAGE(std::filesystem::exists(t_filePath), t_filePath + " file not found!");

    std::ios_base::openmode openMode = std::ios::in;
    if (t_mode & FileMode::Read)
    {
        openMode = std::ios::in;
    }
    if (t_mode & FileMode::Write)
    {
        openMode |= std::ios::out;
    }
    if (t_mode & FileMode::Append)
    {
        openMode |= std::ios::app;
    }
    if (t_mode & FileMode::Trunc)
    {
        openMode |= std::ios::trunc;
    }
    if (t_mode & FileMode::Binary)
    {
        openMode |= std::ios::binary;
    }

    m_fileStream.open(t_filePath, openMode);
}

Rhygine::StandardFile::~StandardFile()
{
    if (m_fileStream.is_open())
    {
        m_fileStream.close();
    }
}

size_t Rhygine::StandardFile::Read(void* buffer, size_t size)
{
    m_fileStream.read(static_cast<char*>(buffer), size);
    return m_fileStream.gcount();
}

std::istream& Rhygine::StandardFile::GetInputStream()
{
    return m_fileStream;
}

size_t Rhygine::StandardFile::Write(const void* buffer, size_t size)
{
    m_fileStream.write(static_cast<const char*>(buffer), size);
    return size;
}

std::ostream& Rhygine::StandardFile::GetOutputStream()
{
    return m_fileStream;
}

void Rhygine::StandardFile::Seek(long offset, Rhygine::SeekOrigin origin)
{
    std::ios_base::seekdir seekDir;
    switch (origin)
    {
        case SeekOrigin::Begin:
            seekDir = std::ios_base::beg;
            break;
        case SeekOrigin::Current:
            seekDir = std::ios_base::cur;
            break;
        case SeekOrigin::End:
            seekDir = std::ios_base::end;
            break;
        default:
            STOP_EXECUTION_MESSAGE("Unimplemented SeekOrigin!");
            return;
    }

    m_fileStream.seekg(offset, seekDir);
    m_fileStream.seekp(offset, seekDir);
}

size_t Rhygine::StandardFile::Size()
{
    if (!m_fileStream.is_open())
    {
        return 0;
    }

    return std::filesystem::file_size(m_filePath);
}

bool Rhygine::StandardFile::Good() const
{
    return m_fileStream.good();
}
