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
			toml::v3::node_view<toml::v3::node> value(m_table);
			size_t start = 0;
			size_t end = key.find('.');

			while (end != std::string::npos)
			{
				const std::string k = key.substr(start, end - start);
				auto newValue = value.at_path(k);
				if (newValue)
				{
					value = newValue;
				}
				else
				{
					return {};
				}
				start = end + 1;
				end = key.find('.', start);
			}

			const std::string k = key.substr(start);
			value = value.at_path(k);
			if (!value)
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
