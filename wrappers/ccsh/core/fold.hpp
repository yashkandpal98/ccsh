#ifndef CCSH_CORE_FOLD_HPP_INCLUDED
#define CCSH_CORE_FOLD_HPP_INCLUDED

#include <ccsh/ccsh_command.hpp>
#include <ccsh/ccsh_wrappers.hpp>

namespace ccsh {


class fold_t : public wrappers::options_paths<fold_t>
{
    using base = wrappers::options_paths<fold_t>;
    friend base;
    static constexpr const char * name() { return "fold"; }

public:

    using base::base;

    CCSH_WRAPPER_ARG0(fold_t, b, "-b")
    CCSH_WRAPPER_ARG0(fold_t, bytes, "--bytes")

    CCSH_WRAPPER_ARG0(fold_t, s, "-s")
    CCSH_WRAPPER_ARG0(fold_t, spaces, "--spaces")

    CCSH_WRAPPER_ARG1_S(fold_t, w, "-w", unsigned, std::to_string(arg))
    CCSH_WRAPPER_ARG1_FWD(fold_t, width, w, unsigned)

};

using fold = command_holder<fold_t>;

}  // namespace ccsh

#endif // CCSH_CORE_FOLD_HPP_INCLUDED
