#ifndef CCSH_INTERNALS_HPP_INCLUDED
#define CCSH_INTERNALS_HPP_INCLUDED

#include <ccsh/ccsh_utils.hpp>

#include <string>
#include <type_traits>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>

namespace ccsh {
namespace internal {

template<typename FUNC>
int retry_handler(FUNC&& func)
{
    int result;
    int err;
    while ((result = func()) == -1 && ((err = errno), err == EINTR || err == EAGAIN || err == EWOULDBLOCK));
    return result;
}

#define CCSH_RETRY_HANDLER(operation) retry_handler([&]() -> int { return (operation); })

inline void stdc_thrower(std::intmax_t result)
{
    if (result == -1)
        throw stdc_error();
}

inline void stdc_thrower(std::intmax_t result, std::string const& msg)
{
    if (result == -1)
        throw stdc_error(errno, msg);
}

inline int shell_logic_or(int a, int b)
{
    return a == 0 ? b : a;
}

inline void close_fd(int fd) noexcept
{
    CCSH_RETRY_HANDLER(close(fd));
}

constexpr mode_t fopen_w_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

constexpr int fopen_flags(stdfd fd, bool append = false)
{
    return fd == stdfd::in ? O_RDONLY :
           (append ?
            (O_WRONLY | O_CREAT | O_APPEND) :
            (O_WRONLY | O_CREAT | O_TRUNC));
}

inline ssize_t mapping_appender(std::string& str, char* buf, std::size_t s)
{
    str.append(buf, s);
    return s;
}

template<typename FUNC>
class line_splitter
{
    std::string temp;
    FUNC func;
    char delim;
public:
    explicit line_splitter(FUNC func, char delim = '\n')
        : func(std::move(func))
        , delim(delim)
    { }

    ssize_t operator()(char* buf, std::size_t s)
    {
        char* newline;
        std::size_t si = s;
        while (si > 0 && (newline = static_cast<char*>(memchr(buf, delim, si))) != nullptr)
        {
            std::size_t diff = newline - buf;
            temp.append(buf, diff);
            // if you want efficient processing, func can take std::string&& argument
            func(std::move(temp));
            temp.clear();
            buf += diff + 1;
            si -= diff + 1;
        }
        temp.append(buf, si);
        return s;
    }
};

template<typename FUNC>
line_splitter<typename std::remove_reference<FUNC>::type> line_splitter_make(FUNC&& func, char delim = '\n')
{   // Comes handy when you have a lambda.
    return line_splitter<typename std::remove_reference<FUNC>::type>(std::forward<FUNC>(func), delim);
}

template<typename FUNC>
void tokenize_string(std::string const& str, std::string const& delimiters, FUNC&& func)
{
    std::string line = str;
    char* saveptr = nullptr;
    char* token = strtok_r(&line[0], delimiters.c_str(), &saveptr);

    while (token != nullptr)
    {
        func(token);
        token = strtok_r(nullptr, delimiters.c_str(), &saveptr);
    }
}

} // namespace internal
} // namespace ccsh

#endif // CCSH_INTERNALS_HPP_INCLUDED
