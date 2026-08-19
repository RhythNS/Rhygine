#pragma once

#include <string>

namespace Rhygine
{
    struct IOError
    {
        enum class Kind
        {
            FileFailed,
            ParseFailed,
            MissingKey,
            TypeMismatch,
            Custom
        };

        Kind        kind;
        std::string message;
        std::string key;
        std::string filePath;

        static IOError MissingKey(const std::string& t_key)
        {
            return { Kind::MissingKey, "Missing required key: " + t_key, t_key, {} };
        }

        static IOError TypeMismatch(const std::string& t_key,
                                    const std::string& t_expected,
                                    const std::string& t_actual)
        {
            return { Kind::TypeMismatch,
                     "Key '" + t_key + "': expected " + t_expected + ", got " + t_actual,
                     t_key, {} };
        }

        static IOError Parse(const std::string& t_detail)
        {
            return { Kind::ParseFailed, t_detail, {}, {} };
        }

        static IOError File(const std::string& t_path, const std::string& t_detail)
        {
            return { Kind::FileFailed, t_detail, {}, t_path };
        }

        static IOError Custom(const std::string& t_message)
        {
            return { Kind::Custom, t_message, {}, {} };
        }
    };
}
