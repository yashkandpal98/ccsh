#ifndef CCSH_CCSH_REGEX_HPP
#define CCSH_CCSH_REGEX_HPP

#include "ccsh_utils.hpp"

#include <regex>

namespace ccsh {

namespace regex_type = std::regex_constants;

template<regex_type::syntax_option_type TYPE>
class regex
{
    std::string source;
    std::regex ex;

public:

    explicit regex(const char* str)
        : source(str)
        , ex(str, TYPE)
    { }

    explicit regex(std::string const& str)
        : source(str)
        , ex(str, TYPE)
    { }

    regex(const char* str, std::size_t count)
        : source(str, count)
        , ex(str, count, TYPE)
    { }

    template<class ForwardIt>
    regex(ForwardIt first, ForwardIt last)
        : source(first, last)
        , ex(first, last, TYPE)
    { }

    regex(std::initializer_list<char> init)
        : source(init)
        , ex(init, TYPE)
    { }

    operator std::string const&() const
    {
        return source;
    }

    operator std::regex const&() const
    {
        return ex;
    }
};

} // namespace ccsh

#endif // CCSH_CCSH_REGEX_HPP
