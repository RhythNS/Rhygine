#pragma once

#include <concepts>
#include <expected>

#include "DataTypes/PropertyMap.h"
#include "StructuredIO/IOError.h"

namespace Rhygine
{
    // Primary template — intentionally undefined.
    // Specialize this for each type you want to serialize/deserialize.
    //
    // Required static methods in the specialization:
    //
    //   static void Serialize(const T& obj, PropertyMap& map);
    //   static std::expected<void, IOError> Deserialize(const PropertyMap& map, T& obj);
    //
    template <typename T>
    struct Describe;

    // ---- Concepts for compile-time detection ----

    template <typename T>
    concept HasDescribe = requires(const T& t_constObj, T& t_obj, PropertyMap& t_map) {
        { Describe<T>::Serialize(t_constObj, t_map) } -> std::same_as<void>;
        { Describe<T>::Deserialize(t_map, t_obj) } -> std::same_as<std::expected<void, IOError>>;
    };
}
