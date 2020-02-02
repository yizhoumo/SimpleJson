cmake_minimum_required(VERSION 3.10)
project(SimpleJson)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_WITH_CODE_COVERAGE "Enable code coverage" OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    if (BUILD_WITH_CODE_COVERAGE)
        add_compile_options(--coverage)
        add_link_options(--coverage)
    endif ()
endif ()

include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
