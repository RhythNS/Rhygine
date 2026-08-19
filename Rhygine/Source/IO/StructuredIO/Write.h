#pragma once

#include <expected>

#include "StructuredIO/Describe.h"
#include "StructuredIO/IOError.h"
#include "StructuredIO/Codecs/TomlCodec.h"
#include "File/File.h"

namespace Rhygine::IO
{
    /// Write an object to file.  Codec defaults to TomlCodec.
    /// TODO: When more codecs are added, select based on file extension.
    template <HasDescribe T, typename CodecT = TomlCodec>
    std::expected<void, IOError> Write(File& t_file, const T& t_obj)
    {
        PropertyMap map;
        Describe<T>::Serialize(t_obj, map);
        return CodecT::Encode(t_file, map);
    }
}
