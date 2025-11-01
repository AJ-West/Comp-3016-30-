cmake_minimum_required(VERSION 3.14)
project(BeatDungeonTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Alternative CMakeLists.txt if FetchContent fails
# This version downloads Google Test manually

# Google Test - Manual download option
set(GTEST_VERSION "1.14.0")
set(GTEST_URL "https://github.com/google/googletest/archive/refs/tags/v${GTEST_VERSION}.zip")
set(GTEST_DIR "${CMAKE_BINARY_DIR}/googletest")

# Download Google Test if not present
if(NOT EXISTS "${GTEST_DIR}/CMakeLists.txt")
    message(STATUS "Downloading Google Test...")
    file(DOWNLOAD ${GTEST_URL} "${CMAKE_BINARY_DIR}/googletest.zip")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_BINARY_DIR}/googletest.zip"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    file(RENAME "${CMAKE_BINARY_DIR}/googletest-${GTEST_VERSION}" "${GTEST_DIR}")
    file(REMOVE "${CMAKE_BINARY_DIR}/googletest.zip")
endif()

# Add Google Test
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

# Include directories - adjust paths relative to tests directory
include_directories(
    ${CMAKE_SOURCE_DIR}/../Beat dungeon
    ${CMAKE_SOURCE_DIR}/../dependencies/SDL/include
    ${CMAKE_SOURCE_DIR}/../dependencies/SDL_ttf/include
    ${CMAKE_SOURCE_DIR}/../dependencies/SDL_image-release-3.2.4/include
    ${CMAKE_SOURCE_DIR}/../dependencies/irrKlang/include
)

# Test files
add_executable(BeatDungeonTests
    test_gameObject.cpp
    test_playerObject.cpp
    test_monsterObject.cpp
    test_pathfinding.cpp
    test_collision.cpp
    test_base64.cpp
    test_keyTime.cpp
    test_button.cpp
    test_particles.cpp
    test_main.cpp
)

# Link libraries
target_link_libraries(BeatDungeonTests
    PRIVATE
    gtest
    gtest_main
)

# Also compile the project's implementation files into the test binary so tests link
# against the same code (exclude the real game entry point main.cpp).
file(GLOB BEAT_SRC "${CMAKE_SOURCE_DIR}/../Beat dungeon/*.cpp")
list(FILTER BEAT_SRC EXCLUDE REGEX ".*main.cpp$")
if(BEAT_SRC)
    message(STATUS "Adding project sources to tests: ${BEAT_SRC}")
    target_sources(BeatDungeonTests PRIVATE ${BEAT_SRC})
endif()

# If prebuilt SDL/irrKlang libraries exist in the project build output, link them.
set(PROJ_LIB_DIR "${CMAKE_SOURCE_DIR}/../Beat dungeon/x64/Debug")
if(EXISTS "${PROJ_LIB_DIR}/SDL3.lib")
    message(STATUS "Linking SDL libraries from: ${PROJ_LIB_DIR}")
    target_link_libraries(BeatDungeonTests PRIVATE
        "${PROJ_LIB_DIR}/SDL3.lib"
        "${PROJ_LIB_DIR}/SDL3_image.lib"
        "${PROJ_LIB_DIR}/SDL3_ttf.lib"
    )
endif()

set(IRR_LIB_DIR "${CMAKE_SOURCE_DIR}/../dependencies/irrKlang/lib/Winx64-visualStudio")
if(EXISTS "${IRR_LIB_DIR}/irrKlang.lib")
    message(STATUS "Linking irrKlang from: ${IRR_LIB_DIR}")
    target_link_libraries(BeatDungeonTests PRIVATE "${IRR_LIB_DIR}/irrKlang.lib")
endif()

# Copy runtime DLLs from the project's x64/Debug output so the test executable can run
set(PROJ_DLL_DIR "${CMAKE_SOURCE_DIR}/../Beat dungeon/x64/Debug")
if(EXISTS "${PROJ_DLL_DIR}")
    file(GLOB RUNTIME_DLLS "${PROJ_DLL_DIR}/*.dll")
    foreach(_dll ${RUNTIME_DLLS})
        add_custom_command(TARGET BeatDungeonTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_dll}" "$<TARGET_FILE_DIR:BeatDungeonTests>"
        )
    endforeach()
endif()

# Enable testing
enable_testing()
include(GoogleTest)
gtest_discover_tests(BeatDungeonTests)

