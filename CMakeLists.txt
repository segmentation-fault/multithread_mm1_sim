cmake_minimum_required(VERSION 3.14)  # 3.14+ for FetchContent + Catch2 integration
project(mm1_sim)

set(CMAKE_CXX_STANDARD 11)

# ---------------------
# Library: mm1_core
# ---------------------
add_library(mm1_core
        src/mm1_sim/Message.cpp
        src/mm1_sim/Channel.cpp
        src/mm1_sim/GeneratorAgent.cpp
        include/mm1_sim/ServerAgent.h
        src/mm1_sim/ServerAgent.cpp
        include/mm1_sim/SinkAgent.h
        include/mm1_sim/Deprecated.h
        include/mm1_sim/ThreadSafeRunningStats.h
)

target_include_directories(mm1_core PUBLIC include)

target_link_libraries(mm1_core
        spdlog::spdlog
)

# ---------------------
# Fetch Catch2
# ---------------------
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

# ---------------------
# Fetch spdlog
# ---------------------
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0  # or latest release
)

FetchContent_MakeAvailable(spdlog)

# ---------------------
# Tests
# ---------------------
enable_testing()

add_executable(mm1_tests
        tests/ChannelTest.cpp
        tests/SinkAgentTest.cpp
)

target_include_directories(mm1_tests PRIVATE ${catch2_SOURCE_DIR}/src)
target_link_libraries(mm1_tests Catch2::Catch2WithMain)

target_link_libraries(mm1_tests
        mm1_core
        Catch2::Catch2WithMain
        spdlog::spdlog
)

include(Catch)
catch_discover_tests(mm1_tests)

# ---------------------
# Main Executable
# ---------------------
add_executable(mm1_sim main.cpp)
target_link_libraries(mm1_sim mm1_core spdlog::spdlog)
