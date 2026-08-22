cmake_minimum_required(VERSION 3.14)
project(Chess_Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(engine/include)

# --- Core engine library (everything except main.cpp) ---
add_library(chess_engine_lib
        engine/include/board.h
        engine/include/evaluation.h
        engine/include/search.h
        engine/src/board.cpp
        engine/src/evaluation.cpp
        engine/src/search.cpp)

# --- Actual executable ---
add_executable(chess_engine
        engine/src/main.cpp)
target_link_libraries(chess_engine PRIVATE chess_engine_lib)

set_target_properties(chess_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>" )

# --- Tests ---
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(tests
            tests/test_movegen.cpp)
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain chess_engine_lib)

    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif()

# --- Snapshot: archive the current build for future engine-vs-engine testing ---
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
endif()
if(NOT GIT_HASH)
    set(GIT_HASH "nogit")
endif()

set(VERSION_TAG "" CACHE STRING "Optional label for a snapshot (e.g. v1-negamax). Defaults to git hash.")
if(VERSION_TAG)
    set(SNAPSHOT_NAME "chess_engine_${VERSION_TAG}")
else()
    set(SNAPSHOT_NAME "chess_engine_${GIT_HASH}")
endif()

add_custom_target(snapshot
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/bin/versions"
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:chess_engine>"
        "${CMAKE_SOURCE_DIR}/bin/versions/${SNAPSHOT_NAME}$<TARGET_FILE_SUFFIX:chess_engine>"
        DEPENDS chess_engine
        COMMENT "Archiving current chess_engine build to bin/versions/"
)