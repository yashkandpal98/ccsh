#ifndef CCSH_CORE_FMT_HPP_INCLUDED
#define CCSH_CORE_FMT_HPP_INCLUDED

#include <ccsh/ccsh_command.hpp>
#include <ccsh/ccsh_wrappers.hpp>

namespace ccsh {


class fmt_t : public wrappers::options_paths<fmt_t>
{
    using base = wrappers::options_paths<fmt_t>;
    friend base;
    static constexpr const char * name() { return "fmt"; }

public:

    using base::base;

    CCSH_WRAPPER_ARG0(fmt_t, c, "-c")
    CCSH_WRAPPER_ARG0(fmt_t, crown_margin, "--crown-margin")

    CCSH_WRAPPER_ARG0(fmt_t, t, "-t")
    CCSH_WRAPPER_ARG0(fmt_t, tagged_paragraph, "--tagged-paragraph")

    CCSH_WRAPPER_ARG0(fmt_t, s, "-s")
    CCSH_WRAPPER_ARG0(fmt_t, split_only, "--split-only")

    CCSH_WRAPPER_ARG0(fmt_t, u, "-u")
    CCSH_WRAPPER_ARG0(fmt_t, uniform_spacing, "--uniform-spacing")

    CCSH_WRAPPER_ARG1_S(fmt_t, w, "-w", unsigned, std::to_string(arg))
    CCSH_WRAPPER_ARG1_FWD(fmt_t, width, w, unsigned)

    CCSH_WRAPPER_ARG1_S(fmt_t, g, "-g", unsigned, std::to_string(arg))
    CCSH_WRAPPER_ARG1_FWD(fmt_t, goal, g, unsigned)

    CCSH_WRAPPER_ARG1_S(fmt_t, p, "-p", std::string, std::move(arg))
    CCSH_WRAPPER_ARG1_FWD(fmt_t, prefix, p, std::string)

};

using fmt = command_holder<fmt_t>;

}  // namespace ccsh

#endif // CCSH_CORE_FMT_HPP_INCLUDED
