#include "ccsh_internals.hpp"
#include <ccsh/ccsh_utils.hpp>

#include <climits>
#include <cstring>

#include <memory>

namespace ccsh {

namespace fs {

std::vector<path> expand(path const& p)
{
    std::vector<path> result;
    internal::expand_helper(p, result);
    return result;
}

std::vector<path> expand(std::vector<path> const& paths)
{
    std::vector<path> result;
    for (path const& p : paths)
        internal::expand_helper(p, result);
    return result;
}

} // namespace fs

fs::path get_current_path()
{
    return fs::current_path();
}

fs::path get_current_directory()
{
    fs::path wd = get_current_path();
    if (wd == get_home())
        return "~";
    return wd.filename();
}

fs::path get_current_path_abbreviated()
{
    fs::path wd = get_current_path();
    fs::path home = get_home();
    if(wd == home)
        return fs::path("~");
    fs::error_code errcode;
    auto abb_wd = fs::relative(wd, home, errcode);
    if (errcode)
        return wd;
    return fs::path("~") / abb_wd;
}

std::string get_short_hostname()
{
    std::string hn = get_hostname();
    auto pos = hn.find('.');
    if (pos == std::string::npos)
        return hn;
    return hn.substr(0, pos);
}

std::string get_shell_name()
{
    return "ccsh";
}

stdc_error::stdc_error(int no)
    : shell_error(std::strerror(errno))
    , error_number(no)
{ }

stdc_error::stdc_error(int no, std::string const& msg)
    : shell_error(msg.empty() ? std::strerror(no) : msg + ": " + strerror(no))
    , error_number(no)
{ }

env_var::operator std::string() const
{
    const char* result = try_get(name);
    return result == nullptr ? "" : result;
}

env_var& env_var::operator=(std::string const& str)
{
    set(name, str);
    return *this;
}

} // namespace ccsh
