#include "PropertyMap.h"

#include <algorithm>
#include <tracy/Tracy.hpp>

#include "Debug/Error.h"

// ---- Private helpers ----

void Rhygine::PropertyMap::SetRaw(const std::string& t_key, Value&& t_value)
{
    ZoneScoped;
    for (auto& [key, value] : m_entries)
    {
        if (key == t_key)
        {
            value = std::move(t_value);
            return;
        }
    }
    m_entries.emplace_back(t_key, std::move(t_value));
}

const Rhygine::PropertyMap::Value* Rhygine::PropertyMap::Find(const std::string& t_key) const
{
    for (const auto& [key, value] : m_entries)
    {
        if (key == t_key) return &value;
    }
    return nullptr;
}

// ---- Setters ----

void Rhygine::PropertyMap::Set(const std::string& t_key, bool t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, int64_t t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, double t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const std::string& t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, std::string&& t_value)
{
    SetRaw(t_key, Value(std::move(t_value)));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const char* t_value)
{
    SetRaw(t_key, Value(std::string(t_value)));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const glm::vec2& t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const glm::vec3& t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const glm::vec4& t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, const PropertyMap& t_value)
{
    SetRaw(t_key, Value(t_value));
}

void Rhygine::PropertyMap::Set(const std::string& t_key, PropertyMap&& t_value)
{
    SetRaw(t_key, Value(std::move(t_value)));
}

// ---- Get (asserting) ----
// Explicit instantiations are in the .cpp to keep the template definition here.
// The generic definition uses TryGet internally.

namespace Rhygine
{
    template <typename T>
    T PropertyMap::Get(const std::string& t_key) const
    {
        ZoneScoped;
        auto opt = TryGet<T>(t_key);
        ASSERT_ERROR_MESSAGE(opt.has_value(),
            "PropertyMap::Get failed — key not found or type mismatch: " + t_key);
        return *opt;
    }

    // Explicit instantiations for all supported scalar types.
    template bool        PropertyMap::Get<bool>(const std::string&) const;
    template int64_t     PropertyMap::Get<int64_t>(const std::string&) const;
    template double      PropertyMap::Get<double>(const std::string&) const;
    template std::string PropertyMap::Get<std::string>(const std::string&) const;
    template glm::vec2   PropertyMap::Get<glm::vec2>(const std::string&) const;
    template glm::vec3   PropertyMap::Get<glm::vec3>(const std::string&) const;
    template glm::vec4   PropertyMap::Get<glm::vec4>(const std::string&) const;
    template PropertyMap PropertyMap::Get<PropertyMap>(const std::string&) const;
}

// ---- Sub-map access ----

const Rhygine::PropertyMap* Rhygine::PropertyMap::TryGetSubMap(const std::string& t_key) const
{
    const Value* val = Find(t_key);
    if (!val) return nullptr;
    return std::get_if<PropertyMap>(val);
}

const Rhygine::PropertyMap& Rhygine::PropertyMap::GetSubMap(const std::string& t_key) const
{
    ZoneScoped;
    const PropertyMap* sub = TryGetSubMap(t_key);
    ASSERT_ERROR_MESSAGE(sub != nullptr,
        "PropertyMap::GetSubMap failed — key not found or not a sub-map: " + t_key);
    return *sub;
}

// ---- Queries ----

bool Rhygine::PropertyMap::Has(const std::string& t_key) const
{
    return Find(t_key) != nullptr;
}

bool Rhygine::PropertyMap::Remove(const std::string& t_key)
{
    ZoneScoped;
    auto it = std::find_if(m_entries.begin(), m_entries.end(),
        [&t_key](const auto& entry) { return entry.first == t_key; });
    if (it == m_entries.end()) return false;
    m_entries.erase(it);
    return true;
}

size_t Rhygine::PropertyMap::Size() const
{
    return m_entries.size();
}

bool Rhygine::PropertyMap::Empty() const
{
    return m_entries.empty();
}

void Rhygine::PropertyMap::Clear()
{
    m_entries.clear();
}
