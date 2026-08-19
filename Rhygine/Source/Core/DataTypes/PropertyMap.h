#pragma once

#include <cstdint>
#include <expected>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "DataTypes/Math.h"

namespace Rhygine
{
    class PropertyMap
    {
    public:
        using Value = std::variant<
            bool,
            int64_t,
            double,
            std::string,
            glm::vec2,
            glm::vec3,
            glm::vec4,
            PropertyMap,
            std::vector<bool>,
            std::vector<int64_t>,
            std::vector<double>,
            std::vector<std::string>,
            std::vector<glm::vec2>,
            std::vector<glm::vec3>,
            std::vector<glm::vec4>,
            std::vector<PropertyMap>
        >;

        // ---- Setters ----
        void Set(const std::string& t_key, bool t_value);
        void Set(const std::string& t_key, int64_t t_value);
        void Set(const std::string& t_key, double t_value);
        void Set(const std::string& t_key, const std::string& t_value);
        void Set(const std::string& t_key, std::string&& t_value);
        void Set(const std::string& t_key, const char* t_value);
        void Set(const std::string& t_key, const glm::vec2& t_value);
        void Set(const std::string& t_key, const glm::vec3& t_value);
        void Set(const std::string& t_key, const glm::vec4& t_value);
        void Set(const std::string& t_key, const PropertyMap& t_value);
        void Set(const std::string& t_key, PropertyMap&& t_value);

        // ---- Array setters ----
        template <typename T>
        void SetArray(const std::string& t_key, std::vector<T>&& t_values)
        {
            SetRaw(t_key, Value(std::move(t_values)));
        }

        template <typename T>
        void SetArray(const std::string& t_key, const std::vector<T>& t_values)
        {
            SetRaw(t_key, Value(t_values));
        }

        // ---- Getters ----
        template <typename T>
        [[nodiscard]] std::optional<T> TryGet(const std::string& t_key) const
        {
            const Value* val = Find(t_key);
            if (!val) return std::nullopt;
            const T* ptr = std::get_if<T>(val);
            if (!ptr) return std::nullopt;
            return *ptr;
        }

        template <typename T>
        [[nodiscard]] T Get(const std::string& t_key) const;

        template <typename T>
        [[nodiscard]] T GetOr(const std::string& t_key, const T& t_fallback) const
        {
            auto opt = TryGet<T>(t_key);
            return opt.has_value() ? *opt : t_fallback;
        }

        // ---- Array getters ----
        template <typename T>
        [[nodiscard]] std::optional<std::vector<T>> TryGetArray(const std::string& t_key) const
        {
            return TryGet<std::vector<T>>(t_key);
        }

        // ---- Sub-map access ----
        [[nodiscard]] const PropertyMap* TryGetSubMap(const std::string& t_key) const;
        [[nodiscard]] const PropertyMap& GetSubMap(const std::string& t_key) const;

        // ---- Queries ----
        [[nodiscard]] bool Has(const std::string& t_key) const;
        bool Remove(const std::string& t_key);
        [[nodiscard]] size_t Size() const;
        [[nodiscard]] bool Empty() const;
        void Clear();

        // ---- Iteration ----
        // Visitor signature: void(const std::string& key, const Value& value)
        template <typename Fn>
        void ForEach(Fn&& t_visitor) const
        {
            for (const auto& [key, value] : m_entries)
            {
                t_visitor(key, value);
            }
        }

        bool operator==(const PropertyMap& t_other) const = default;

    private:
        void SetRaw(const std::string& t_key, Value&& t_value);
        [[nodiscard]] const Value* Find(const std::string& t_key) const;

        // Ordered storage — preserves insertion order for deterministic output.
        // O(n) lookup is fine for typical property counts (< 50).
        std::vector<std::pair<std::string, Value>> m_entries;
    };
}
