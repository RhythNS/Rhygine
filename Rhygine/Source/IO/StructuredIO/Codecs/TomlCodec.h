#pragma once

#include <expected>

#include "DataTypes/PropertyMap.h"
#include "StructuredIO/IOError.h"

namespace Rhygine
{
    class File;

    class TomlCodec
    {
    public:
        [[nodiscard]] static std::expected<PropertyMap, IOError> Decode(File& t_file);
        static std::expected<void, IOError> Encode(File& t_file, const PropertyMap& t_map);
    };
}
