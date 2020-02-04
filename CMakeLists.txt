cmake_minimum_required(VERSION 3.10)
project(SimpleJson)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(CODE_COVERAGE "Enable code coverage" OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    if (CODE_COVERAGE)
        add_compile_options(--coverage)
        link_libraries(--coverage)
    endif ()
endif ()

include_directories(include)
add_subdirectory(src)

# Build tests only if this is the top-level project
if (BUILD_TESTS AND (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))
    enable_testing()
    add_subdirectory(test)
endif ()
