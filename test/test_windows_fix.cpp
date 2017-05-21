#include <gtest/gtest.h>

#ifdef _WIN32

#pragma comment(lib, "googletest_v140.lib")

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // _WIN32
