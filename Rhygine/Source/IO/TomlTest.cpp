#include <toml++/toml.hpp>
#include <iostream>
#include <sstream>

void Test()
{
    using namespace std::string_view_literals;
    constexpr std::string_view some_toml = R"(
        [library]
        name = "toml++"
        authors = ["Mark Gillard"]
        cpp = 17
    )"sv;
    try
    {
        // parse directly from a string view:
        {
            toml::table tbl = toml::parse(some_toml);
            std::cout << tbl << "\n";

        }

        // parse from a string stream:
        {
            std::stringstream ss{ std::string{ some_toml } };
            toml::table tbl = toml::parse(ss);
            std::cout << tbl << "\n";
        }
    }
    catch (const toml::parse_error& err)
    {
        std::cerr << "Parsing failed:\n" << err << "\n";
    }
}
