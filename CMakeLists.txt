cmake_minimum_required(VERSION 3.16)
project(RookieEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAG_RELEASE "-03 -march=native -flto -DNDEBUG")

set(SOURCES
    core/main.cpp
    core/attackglobals.cpp
    core/utils.cpp
    core/magics.cpp
    core/magicutils.cpp
    core/movegen.cpp
    core/moveexec.cpp
    core/moveval.cpp
    core/threatgen.cpp
    core/timer.cpp
    core/test.cpp
    core/evaluate.cpp
    core/moveorder.cpp
    core/search.cpp
    core/engine.cpp
    core/legalmove.cpp
)

add_executable(Rookie ${SOURCES})

target_include_directories(Rookie PRIVATE
    ${PROJECT_SOURCE_DIR}/core
)

add_custom_command(TARGET Rookie POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:Rookie>/data
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Rookie PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Release>:-O3>
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Rookie PRIVATE
        /W4
        $<$<CONFIG:Release>:/Ox>
    )
endif()