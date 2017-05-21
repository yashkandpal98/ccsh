cmake_minimum_required (VERSION 2.6)

add_subdirectory(./../lib ccsh_lib)

include_directories(${SHARED_LIB_INCLUDE_DIR})
include_directories(${WRAPPERS_INCLUDE_DIR})

file(GLOB_RECURSE wrappers ../wrappers/*.hpp)
file(GLOB_RECURSE tests test*.cpp)

enable_testing()

if(WIN32)
    set(GTEST_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/../packages/googletest.1.8.0.0/build/native")
    add_definitions(-DGTEST_HAS_STD_TUPLE_)
    add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
    include_directories(${GTEST_PREFIX}/include)
    link_directories(${GTEST_PREFIX}/lib/Win32/v140/Debug)
    set(GTEST_LIBS googletest_v140.lib)
else()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    set(GTEST_LIBS ${GTEST_BOTH_LIBRARIES})
endif()

include_directories(${GTEST_INCLUDE_DIRS})
add_executable(ccsh_test ${tests} ${wrappers})
target_link_libraries( ccsh_test PUBLIC ccsh_lib ${filesystem_lib})
