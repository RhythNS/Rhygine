#include "File.h"

Rhygine::File::File(std::string& t_filePath, FileMode t_mode)
	: m_filePath(t_filePath), m_fileMode(t_mode)
{
}

std::vector<char> Rhygine::File::ReadAll()
{
    size_t fileSize = Size();
    std::vector<char> buffer(fileSize);
    Read(buffer.data(), fileSize);
    return buffer;
}

void Rhygine::File::WriteAllBytes(const std::vector<char>& data)
{
    Write(data.data(), data.size());
}
