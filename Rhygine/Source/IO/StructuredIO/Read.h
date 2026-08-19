#pragma once

#include <expected>
#include <string>

#include "StructuredIO/Describe.h"
#include "StructuredIO/IOError.h"
#include "StructuredIO/Codecs/TomlCodec.h"
#include "File/File.h"

namespace Rhygine::IO
{
    namespace Detail
    {
        // Extract file extension from the path stored in File.
        // For now, always returns TomlCodec.  Extend this when adding more codecs.
        // The File class stores m_filePath as protected; we rely on the codec being
        // selected externally or defaulting to TOML.

        template <typename CodecT, HasDescribe T>
        std::expected<void, IOError> ReadWithCodec(File& t_file, T& t_obj)
        {
            auto mapResult = CodecT::Decode(t_file);
            if (!mapResult)
            {
                return std::unexpected(mapResult.error());
            }
            return Describe<T>::Deserialize(*mapResult, t_obj);
        }
    }

    /// Read into an existing object.  Codec defaults to TomlCodec.
    /// TODO: When more codecs are added, select based on file extension.
    template <HasDescribe T, typename CodecT = TomlCodec>
    std::expected<void, IOError> Read(File& t_file, T& t_obj)
    {
        return Detail::ReadWithCodec<CodecT, T>(t_file, t_obj);
    }
}
