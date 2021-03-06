#ifndef CCSH_CCSH_OPERATORS_HPP
#define CCSH_CCSH_OPERATORS_HPP

#include "ccsh_command.hpp"
#include <iostream>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-comparison"
#endif

namespace ccsh {

inline internal::command_runnable shell(fs::path const& p, std::vector<std::string> const& args = {})
{
    return {new internal::command_native(p, args)};
}

inline internal::command_runnable source(fs::path const& p, std::vector<std::string> const& args = {})
{
    return {new internal::command_source(p, args)};
}

namespace internal {

inline command_runnable operator|(command const& a, command const& b)
{
    return {new command_pipe(a, b)};
}

template<typename COMMAND>
command_holder <COMMAND>&& operator/(fs::path const& p, command_holder <COMMAND>&& c)
{
    c.append_dir(p);
    return std::move(c);
}

template<typename COMMAND>
command_holder <COMMAND>& operator/(fs::path const& p, command_holder <COMMAND>& c)
{
    c.append_dir(p);
    return c;
}

/* ******************* file redirection operators ******************* */

inline command_runnable operator<(command const& c, fs::path const& p)
{
    return {new command_redirect<stdfd::in>(c, p)};
}

inline command_runnable operator>(command const& c, fs::path const& p)
{
    return {new command_redirect<stdfd::out>(c, p, false)};
}

inline command_runnable operator>>(command const& c, fs::path const& p)
{
    return {new command_redirect<stdfd::out>(c, p, true)};
}

inline command_runnable operator>=(command const& c, fs::path const& p)
{
    return {new command_redirect<stdfd::err>(c, p, false)};
}

inline command_runnable operator>>=(command const& c, fs::path const& p)
{
    return {new command_redirect<stdfd::err>(c, p, true)};
}

/* ******************* file redirection operators ******************* */



/* ******************* string redirection operators ******************* */

command_runnable operator<(command const& c, std::string& str);
command_runnable operator>>(command const& c, std::string& str);
command_runnable operator>(command const& c, std::string& str);
command_runnable operator>>=(command const& c, std::string& str);
command_runnable operator>=(command const& c, std::string& str);

/* ******************* string redirection operators ******************* */


/* ******************* vector redirection operators ******************* */

command_runnable operator<(command const& c, std::vector<std::string>& vec);
command_runnable operator>>(command const& c, std::vector<std::string>& vec);
command_runnable operator>(command const& c, std::vector<std::string>& vec);
command_runnable operator>>=(command const& c, std::vector<std::string>& vec);
command_runnable operator>=(command const& c, std::vector<std::string>& vec);

/* ******************* vector redirection operators ******************* */

/* ******************* line functor redirection operators ******************* */

//command_runnable operator<  (command const& c, command_functor_line func);
command_runnable operator>(command const& c, command_functor_line func);
command_runnable operator>=(command const& c, command_functor_line func);

/* ******************* line functor redirection operators ******************* */


/* ******************* raw functor redirection operators ******************* */

command_runnable operator<(command const& c, command_functor_raw func);
command_runnable operator>(command const& c, command_functor_raw func);
command_runnable operator>=(command const& c, command_functor_raw func);

/* ******************* raw functor redirection operators ******************* */

/* ******************* command functor redirection operators ******************* */

// command_runnable operator<(command const& c, command_functor func);
// command_runnable operator>(command const& c, command_functor func);
// command_runnable operator>=(command const& c, command_functor func);

inline command_runnable operator|(command const& c, command_functor func)
{
    return {new command_pipe{c, command_make(std::move(func))}};
}

inline command_runnable operator|(command_functor func, command const& c)
{
    return {new command_pipe{command_make(std::move(func)), c}};
}

/* ******************* command functor redirection operators ******************* */


/* ******************* logical operators ******************* */

inline command_runnable operator&&(command const& a, command const& b)
{
    return {new command_and(a, b)};
}

inline command_runnable operator||(command const& a, command const& b)
{
    return {new command_or(a, b)};
}

inline command_runnable operator&&(command const& a, bool b)
{
    return {new command_and(a, command_runnable{new command_bool(b)})};
}

inline command_runnable operator||(command const& a, bool b)
{
    return {new command_or(a, command_runnable{new command_bool(b)})};
}

inline command_runnable operator&&(bool b, command const& a) // provided only for consistency, useless anyways
{
    return {new command_and(command_runnable{new command_bool(b)}, a)};
}

inline command_runnable operator||(bool b, command const& a) // provided only for consistency, useless anyways
{
    return {new command_or(command_runnable{new command_bool(b)}, a)};
}

/* ******************* logical operators ******************* */


/* ******************* Sequencing operators ******************* */

inline command_runnable operator,(command const& a, command const& b)
{
    return {new command_comma(a, b)};
}

inline command_runnable operator,(command const& a, bool b)
{
    return {new command_comma(a, command_runnable{new command_bool(b)})};
}

inline command_runnable operator,(bool b, command const& a) // provided only for consistency, useless anyways
{
    return {new command_comma(command_runnable{new command_bool(b)}, a)};
}

/* ******************* Sequencing operators ******************* */



} // namespace internal

namespace literals {
// note the space between `"" _p` for old g++
inline fs::path operator "" _p(const char* str, std::size_t)
{
    return fs::path{str};
}
} // namespace literals


inline env_var dollar(std::string const& name)
{
    return env_var(name);
}

inline std::string dollar(command const& c)
{
    std::string x;
    std::string err;
    if ((c > x >= err).run() != 0)
        throw shell_error(err);
    return x;
}

#ifdef CCSH_NON_STANDARD_DOLLAR

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#endif

inline env_var $(std::string const& name)
{
    return dollar(name);
}

inline std::string $(command const& c)
{
    return dollar(c);
}

#endif

inline std::ostream& operator<<(std::ostream& os, env_var const& var)
{
    os << std::string(var);
    return os;
}
} // namespace ccsh


#endif // CCSH_CCSH_OPERATORS_HPP
