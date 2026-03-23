#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include <optional>
#include <vector>

#include "DataTypes\Concurrency\SharedMutex.h"
#include "Debug\Error.h"

namespace Rhygine
{
	class Config
	{
	public:
		Config() = default;

		template <typename T>
		[[nodiscard]] T Get(const std::string& t_key) const
		{
			std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			ASSERT_ERROR_MESSAGE(m_map.find(t_key) != m_map.end(), "Config tried to access a key which was not in it's map!");
			return std::any_cast<T>(m_map.at(t_key));
		}

		template <typename T>
		[[nodiscard]] std::optional<T> TryGet(const std::string& t_key) const
		{
			std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			if (m_map.find(t_key) == m_map.end())
			{
				return {};
			}
			try
			{
				return std::any_cast<T>(m_map.at(t_key));
			}
			catch (const std::bad_any_cast& e)
			{
				return {};
			}
		}

		template <typename T>
		[[nodiscard]] T GetOr(const std::string& t_key, T t_or) const
		{
			std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			if (m_map.find(t_key) == m_map.end())
			{
				return t_or;
			}
			try
			{
				return std::any_cast<T>(m_map.at(t_key));
			}
			catch (const std::bad_any_cast& e)
			{
				return t_or;
			}
		}

		template <typename T>
		[[nodiscard]] T GetOr(const std::string& t_key, T& t_or) const
		{
			std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			if (m_map.find(t_key) == m_map.end())
			{
				return t_or;
			}
			try
			{
				return std::any_cast<T>(m_map.at(t_key));
			}
			catch (const std::bad_any_cast& e)
			{
				return t_or;
			}
		}

		template <typename T>
		bool Set(const std::string& t_key, T& t_value, bool t_overwrite = false)
		{
			std::unique_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			if (!t_overwrite && m_map.find(t_key) != m_map.end())
			{
				return false;
			}

			m_map[t_key] = std::make_any<T>(t_value);
			return true;
		}

		template <typename T>
		bool Set(const std::string& t_key, T t_value, bool t_overwrite = false)
		{
			std::unique_lock<Rhygine::SharedMutex> lock(m_sharedMutex);
			if (!t_overwrite && m_map.find(t_key) != m_map.end())
			{
				return false;
			}

			m_map[t_key] = std::make_any<T>(t_value);
			return true;
		}

	private:
		mutable SharedMutex m_sharedMutex;
		std::unordered_map<std::string, std::any> m_map;
	};
}
