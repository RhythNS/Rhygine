#include "TomlCodec.h"

#include <tracy/Tracy.hpp>
#include <toml++/toml.hpp>

#include "File/File.h"
#include "Debug/Error.h"

namespace
{
    // ---- Decode helpers: toml → PropertyMap ----

    Rhygine::PropertyMap TableToPropertyMap(const toml::table& t_table);

    void NodeToPropertyMap(const std::string& t_key, const toml::node& t_node,
                           Rhygine::PropertyMap& t_map)
    {
        if (t_node.is_table())
        {
            t_map.Set(t_key, TableToPropertyMap(*t_node.as_table()));
        }
        else if (t_node.is_array())
        {
            const toml::array& arr = *t_node.as_array();
            if (arr.empty())
            {
                // Empty array — store as empty vector<int64_t> (arbitrary choice)
                t_map.SetArray<int64_t>(t_key, {});
                return;
            }

            // Check if this is a glm vector (array of 2-4 numbers, no nested arrays/tables)
            bool allNumbers = arr.size() >= 2 && arr.size() <= 4;
            if (allNumbers)
            {
                for (size_t i = 0; i < arr.size(); ++i)
                {
                    if (!arr[i].is_number()) { allNumbers = false; break; }
                }
            }

            if (allNumbers && arr.size() == 2)
            {
                auto getF = [&](size_t i) -> float {
                    return arr[i].as_floating_point()
                        ? static_cast<float>(arr[i].as_floating_point()->get())
                        : static_cast<float>(arr[i].as_integer()->get());
                };
                t_map.Set(t_key, glm::vec2(getF(0), getF(1)));
                return;
            }
            if (allNumbers && arr.size() == 3)
            {
                auto getF = [&](size_t i) -> float {
                    return arr[i].as_floating_point()
                        ? static_cast<float>(arr[i].as_floating_point()->get())
                        : static_cast<float>(arr[i].as_integer()->get());
                };
                t_map.Set(t_key, glm::vec3(getF(0), getF(1), getF(2)));
                return;
            }
            if (allNumbers && arr.size() == 4)
            {
                auto getF = [&](size_t i) -> float {
                    return arr[i].as_floating_point()
                        ? static_cast<float>(arr[i].as_floating_point()->get())
                        : static_cast<float>(arr[i].as_integer()->get());
                };
                t_map.Set(t_key, glm::vec4(getF(0), getF(1), getF(2), getF(3)));
                return;
            }

            // Homogeneous typed arrays
            if (arr[0].is_integer())
            {
                std::vector<int64_t> values;
                values.reserve(arr.size());
                for (const auto& elem : arr)
                {
                    if (elem.is_integer()) values.push_back(elem.as_integer()->get());
                }
                t_map.SetArray<int64_t>(t_key, std::move(values));
            }
            else if (arr[0].is_floating_point())
            {
                std::vector<double> values;
                values.reserve(arr.size());
                for (const auto& elem : arr)
                {
                    if (elem.is_floating_point()) values.push_back(elem.as_floating_point()->get());
                    else if (elem.is_integer()) values.push_back(static_cast<double>(elem.as_integer()->get()));
                }
                t_map.SetArray<double>(t_key, std::move(values));
            }
            else if (arr[0].is_boolean())
            {
                std::vector<bool> values;
                values.reserve(arr.size());
                for (const auto& elem : arr)
                {
                    if (elem.is_boolean()) values.push_back(elem.as_boolean()->get());
                }
                t_map.SetArray<bool>(t_key, std::move(values));
            }
            else if (arr[0].is_string())
            {
                std::vector<std::string> values;
                values.reserve(arr.size());
                for (const auto& elem : arr)
                {
                    if (elem.is_string()) values.push_back(std::string(elem.as_string()->get()));
                }
                t_map.SetArray<std::string>(t_key, std::move(values));
            }
            else if (arr[0].is_table())
            {
                std::vector<Rhygine::PropertyMap> values;
                values.reserve(arr.size());
                for (const auto& elem : arr)
                {
                    if (elem.is_table()) values.push_back(TableToPropertyMap(*elem.as_table()));
                }
                t_map.SetArray<Rhygine::PropertyMap>(t_key, std::move(values));
            }
            else if (arr[0].is_array())
            {
                const toml::array& firstSub = *arr[0].as_array();
                bool allVec2 = firstSub.size() == 2;
                bool allVec3 = firstSub.size() == 3;
                bool allVec4 = firstSub.size() == 4;
                for (const auto& elem : arr)
                {
                    if (!elem.is_array()) { allVec2 = allVec3 = allVec4 = false; break; }
                    const toml::array& sub = *elem.as_array();
                    if (sub.size() != 2) allVec2 = false;
                    if (sub.size() != 3) allVec3 = false;
                    if (sub.size() != 4) allVec4 = false;
                    for (const auto& n : sub)
                    {
                        if (!n.is_number()) { allVec2 = allVec3 = allVec4 = false; break; }
                    }
                }
                if (allVec2)
                {
                    std::vector<glm::vec2> values;
                    values.reserve(arr.size());
                    for (const auto& elem : arr)
                    {
                        const toml::array& sub = *elem.as_array();
                        auto getSubF = [&](size_t i) -> float {
                            return sub[i].as_floating_point()
                                ? static_cast<float>(sub[i].as_floating_point()->get())
                                : static_cast<float>(sub[i].as_integer()->get());
                        };
                        values.emplace_back(getSubF(0), getSubF(1));
                    }
                    t_map.SetArray<glm::vec2>(t_key, std::move(values));
                }
                else if (allVec3)
                {
                    std::vector<glm::vec3> values;
                    values.reserve(arr.size());
                    for (const auto& elem : arr)
                    {
                        const toml::array& sub = *elem.as_array();
                        auto getSubF = [&](size_t i) -> float {
                            return sub[i].as_floating_point()
                                ? static_cast<float>(sub[i].as_floating_point()->get())
                                : static_cast<float>(sub[i].as_integer()->get());
                        };
                        values.emplace_back(getSubF(0), getSubF(1), getSubF(2));
                    }
                    t_map.SetArray<glm::vec3>(t_key, std::move(values));
                }
                else if (allVec4)
                {
                    std::vector<glm::vec4> values;
                    values.reserve(arr.size());
                    for (const auto& elem : arr)
                    {
                        const toml::array& sub = *elem.as_array();
                        auto getSubF = [&](size_t i) -> float {
                            return sub[i].as_floating_point()
                                ? static_cast<float>(sub[i].as_floating_point()->get())
                                : static_cast<float>(sub[i].as_integer()->get());
                        };
                        values.emplace_back(getSubF(0), getSubF(1), getSubF(2), getSubF(3));
                    }
                    t_map.SetArray<glm::vec4>(t_key, std::move(values));
                }
            }
        }
        else if (t_node.is_boolean())
        {
            t_map.Set(t_key, t_node.as_boolean()->get());
        }
        else if (t_node.is_integer())
        {
            t_map.Set(t_key, t_node.as_integer()->get());
        }
        else if (t_node.is_floating_point())
        {
            t_map.Set(t_key, t_node.as_floating_point()->get());
        }
        else if (t_node.is_string())
        {
            t_map.Set(t_key, std::string(t_node.as_string()->get()));
        }
        // toml date/time types are intentionally not mapped for now.
    }

    Rhygine::PropertyMap TableToPropertyMap(const toml::table& t_table)
    {
        Rhygine::PropertyMap map;
        for (const auto& [key, node] : t_table)
        {
            NodeToPropertyMap(std::string(key), node, map);
        }
        return map;
    }

    // ---- Encode helpers: PropertyMap → toml ----

    toml::table PropertyMapToTable(const Rhygine::PropertyMap& t_map);

    toml::array Vec2ToArray(const glm::vec2& t_v)
    {
        toml::array arr;
        arr.push_back(static_cast<double>(t_v.x));
        arr.push_back(static_cast<double>(t_v.y));
        return arr;
    }

    toml::array Vec3ToArray(const glm::vec3& t_v)
    {
        toml::array arr;
        arr.push_back(static_cast<double>(t_v.x));
        arr.push_back(static_cast<double>(t_v.y));
        arr.push_back(static_cast<double>(t_v.z));
        return arr;
    }

    toml::array Vec4ToArray(const glm::vec4& t_v)
    {
        toml::array arr;
        arr.push_back(static_cast<double>(t_v.x));
        arr.push_back(static_cast<double>(t_v.y));
        arr.push_back(static_cast<double>(t_v.z));
        arr.push_back(static_cast<double>(t_v.w));
        return arr;
    }

    void InsertValueIntoTable(toml::table& t_table, const std::string& t_key,
                              const Rhygine::PropertyMap::Value& t_value)
    {
        std::visit([&](const auto& val)
        {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, bool>)
                t_table.insert_or_assign(t_key, val);
            else if constexpr (std::is_same_v<T, int64_t>)
                t_table.insert_or_assign(t_key, val);
            else if constexpr (std::is_same_v<T, double>)
                t_table.insert_or_assign(t_key, val);
            else if constexpr (std::is_same_v<T, std::string>)
                t_table.insert_or_assign(t_key, val);
            else if constexpr (std::is_same_v<T, glm::vec2>)
                t_table.insert_or_assign(t_key, Vec2ToArray(val));
            else if constexpr (std::is_same_v<T, glm::vec3>)
                t_table.insert_or_assign(t_key, Vec3ToArray(val));
            else if constexpr (std::is_same_v<T, glm::vec4>)
                t_table.insert_or_assign(t_key, Vec4ToArray(val));
            else if constexpr (std::is_same_v<T, Rhygine::PropertyMap>)
                t_table.insert_or_assign(t_key, PropertyMapToTable(val));
            else if constexpr (std::is_same_v<T, std::vector<bool>>)
            {
                toml::array arr;
                for (bool b : val) arr.push_back(b);
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<int64_t>>)
            {
                toml::array arr;
                for (int64_t i : val) arr.push_back(i);
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<double>>)
            {
                toml::array arr;
                for (double d : val) arr.push_back(d);
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<std::string>>)
            {
                toml::array arr;
                for (const auto& s : val) arr.push_back(s);
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<glm::vec2>>)
            {
                toml::array arr;
                for (const auto& v : val) arr.push_back(Vec2ToArray(v));
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<glm::vec3>>)
            {
                toml::array arr;
                for (const auto& v : val) arr.push_back(Vec3ToArray(v));
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<glm::vec4>>)
            {
                toml::array arr;
                for (const auto& v : val) arr.push_back(Vec4ToArray(v));
                t_table.insert_or_assign(t_key, std::move(arr));
            }
            else if constexpr (std::is_same_v<T, std::vector<Rhygine::PropertyMap>>)
            {
                toml::array arr;
                for (const auto& m : val) arr.push_back(PropertyMapToTable(m));
                t_table.insert_or_assign(t_key, std::move(arr));
            }
        }, t_value);
    }

    toml::table PropertyMapToTable(const Rhygine::PropertyMap& t_map)
    {
        toml::table table;
        t_map.ForEach([&](const std::string& key, const Rhygine::PropertyMap::Value& value)
        {
            InsertValueIntoTable(table, key, value);
        });
        return table;
    }
}

// ---- Public API ----

std::expected<Rhygine::PropertyMap, Rhygine::IOError>
Rhygine::TomlCodec::Decode(File& t_file)
{
    ZoneScoped;
    try
    {
        toml::table table = toml::parse(t_file.GetInputStream());
        return TableToPropertyMap(table);
    }
    catch (const toml::parse_error& err)
    {
        return std::unexpected(IOError::Parse(std::string(err.what())));
    }
}

std::expected<void, Rhygine::IOError>
Rhygine::TomlCodec::Encode(File& t_file, const PropertyMap& t_map)
{
    ZoneScoped;
    toml::table table = PropertyMapToTable(t_map);
    auto& os = t_file.GetOutputStream();
    os << table;
    if (!os.good())
    {
        return std::unexpected(IOError::File({}, "Failed to write TOML output"));
    }
    return {};
}
