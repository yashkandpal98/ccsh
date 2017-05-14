#ifndef CCSH_CCSH_UTILS_HPP
#define CCSH_CCSH_UTILS_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <array>
#include <string>

#include "ccsh_types.hpp"
#include "ccsh_filesystem.hpp"

#include "CWrapper.hpp"

namespace ccsh {

namespace internal {

void expand_helper(fs::path const& p, std::vector<fs::path>& result);

}

namespace fs {
std::vector<path> expand(path const& p);
std::vector<path> expand(std::vector<path> const& paths);
} // namespace fs

// NEVER EVER USE boost::filesystem DIRECTLY, ALWAYS USE ccsh::fs
// boost::filesystem WILL BE CHANGED TO std::filesystem WITH C++17

fs::path get_home();
fs::path get_current_path();
fs::path get_current_directory();
fs::path get_current_path_abbreviated();

std::string get_hostname();
std::string get_short_hostname();

std::string get_shell_name();

std::string get_ttyname();

bool is_user_possibly_elevated();

class shell_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class winapi_error : public shell_error
{
    error_t error_number;
public:
    winapi_error();
    explicit winapi_error(error_t no);
    winapi_error(error_t no, std::string const& msg);

    error_t no() const
    { return error_number; }
};

class stdc_error : public shell_error
{
    int error_number;
public:
    explicit stdc_error(int no = errno);
    stdc_error(int no, std::string const& msg);

    int no() const
    { return error_number; }
};


class env_var
{
    std::string name;
public:

    static std::string get(std::string const& name);
    static const char* try_get(std::string const& name); // WARNING: on windows, you should copy the return value if the function succeeds
    static void set(std::string const& name, std::string const& value, bool overwrite = true);
    static int try_set(std::string const& name, std::string const& value, bool overwrite = true);

    explicit env_var(std::string name)
        : name(std::move(name))
    { }

    env_var(env_var&&) = default;
    env_var(env_var const&) = delete;
    env_var& operator=(env_var&&) = default;
    env_var& operator=(env_var const&) = delete;

    operator std::string() const; // getenv
    env_var& operator=(std::string const&); // setenv
};

namespace internal {

struct open_traits
{
#ifdef _WIN32
    static const fd_t invalid_value;
#else
    static constexpr fd_t invalid_value = -1;
#endif
    using exception = fd_exception_t;

    static void dtor_func(fd_t fd) noexcept;
};

using open_wrapper = CW::CWrapper<fd_t, open_traits, CW::CWrapperType::Get, false>;

enum class stdfd : uint8_t
{
    in = 0,
    out = 1,
    err = 2,
    count
};

using pipe_t = std::array<open_wrapper, 2>;

pipe_t pipe_compat();
std::size_t read_compat(fd_t file, void* data, std::size_t size);
std::size_t write_compat(fd_t file, void* data, std::size_t size);
fd_t duplicate_compat(fd_t file);
void close_compat(fd_t fd);

} // namespace internal

} // namespace ccsh

#endif // CCSH_CCSH_UTILS_HPP
