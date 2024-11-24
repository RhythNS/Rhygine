#pragma once

#include <toml++/toml.hpp>

namespace Rhygine
{
	class File;

	class TomlInterpreter
	{
	public:
		template<typename T>
		std::optional<T> GetValue(const std::string& key)
		{
			auto value = m_table;
			size_t start = 0;
			size_t end = key.find('.');

			while (end != -1)
			{
				std::string k = key.substr(start, end - start);
				if (value.contains(k))
				{
					value = value[k];
				}
				else
				{
					return {};
				}
				start = end + 1;
				end = key.find('.', start);
			}

			std::string k = key.substr(start);
			if (value.contains(k))
			{
				value = value[k];
			}
			else
			{
				return {};
			}

			if (value.is<T>())
			{
				return value.as<T>()->get();
			}
			return {};
		}

		template<typename T>
		void SetValue(const std::string& key, const T& value)
		{
			m_table.insert_or_assign(key, value);
		}

		bool Load(File& t_file);
		void Save(File& t_file);

	private:
		toml::table m_table;
	};
}
