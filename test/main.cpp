#include <iostream>
#define CCSH_NON_STANDARD_DOLLAR
#include <ccsh/ccsh.hpp>

#include <ccsh/core.hpp>

using namespace ccsh::literals;

void test1()
{
    using namespace ccsh::core;
    ccsh::command c1 = cat().e();
    auto c2 = c1;
    ccsh::command_builder<cat> x = cat().n();

    x.T();
}

void test2()
{
    using namespace ccsh;
    std::string user = $("USER");
    std::cout << user << " " << user.length() << std::endl;
    $("BAFFER") = "some text";
    std::cout << $("BAFFER") << std::endl;

    shell("echo", {$(shell("ls"))});
}

void test3()
{
    using namespace ccsh::core;
    cat();
}

void test4()
{
    ccsh::core::cat(~"Documents/ccsh/src/main.cpp"_p);
}

int main()
{
    std::cout << ccsh::get_home().string() << std::endl;
    test2();
    test4();
    ccsh::shell("la");
    ccsh::shell("la") | ccsh::shell("cowsay"); // no cow should appear

    ccsh::fs::path f = "/tmp/bs.txt"_p;

    ccsh::command c1 = ccsh::shell("ls", {"-lh"}) > f;
    c1.run();

    ccsh::shell("cat", {f.string()});

    ccsh::shell("ls", {"-lh"}) | ccsh::shell("cowsay") | ccsh::shell("lolcat");

    std::string str;
    ccsh::shell("echo", {"--- ls finished. ---"}) > str;
    std::cout << str.size() << std::endl;
    std::cout << str << std::endl;

    ccsh::shell("lolcat") < str;

    std::vector<std::string> vec;
    ccsh::shell("ls", {"-1"}) > vec;

    for(auto const& s : vec)
        std::cout << s << std::endl;

    // Not compiling, as intended...
    //auto c2 = ccsh::shell("cowsay");


    return 0;
}
