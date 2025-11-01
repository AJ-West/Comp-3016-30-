# Fixing Google Test Download Error

If you're getting errors like:
```
CMake Error: Build step for googletest failed
```

Here are several solutions:

## Solution 1: Use Git Instead of ZIP (Recommended)

The current CMakeLists.txt uses Git. If Git is not available, try Solution 2.

## Solution 2: Manual Download Method

1. **Replace CMakeLists.txt** with the alternative version:
   ```powershell
   cd "beat dungeon\tests"
   copy CMakeLists_alternative.txt CMakeLists.txt
   ```

2. **Clean and rebuild**:
   ```powershell
   if (Test-Path build) { Remove-Item -Recurse -Force build }
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

## Solution 3: Download Google Test Manually

1. **Download Google Test manually**:
   - Go to: https://github.com/google/googletest/releases
   - Download `googletest-1.14.0.zip` (or latest version)
   - Extract it to: `beat dungeon/tests/googletest/`

2. **Modify CMakeLists.txt** - Replace the FetchContent section with:
   ```cmake
   # Use manually downloaded Google Test
   set(GTEST_DIR "${CMAKE_SOURCE_DIR}/googletest")
   if(NOT EXISTS "${GTEST_DIR}/CMakeLists.txt")
       message(FATAL_ERROR "Google Test not found. Please download to ${GTEST_DIR}")
   endif()
   
   set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
   set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
   add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)
   ```

## Solution 4: Use vcpkg (If you have it)

If you have vcpkg installed:

```powershell
vcpkg install gtest:x64-windows
```

Then modify CMakeLists.txt to use:
```cmake
find_package(GTest REQUIRED)
target_link_libraries(BeatDungeonTests PRIVATE GTest::gtest GTest::gtest_main)
```

## Solution 5: Disable Google Test Download (Use Pre-installed)

If Google Test is already installed on your system:

```powershell
cd build
cmake .. -DFETCH_GTEST=OFF
```

## Quick Fix Script

Run this PowerShell script to try Solution 2 automatically:

```powershell
cd "beat dungeon\tests"
copy CMakeLists_alternative.txt CMakeLists.txt
if (Test-Path build) { Remove-Item -Recurse -Force build }
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
```

## Check Your CMake Version

Make sure you have CMake 3.14+:
```powershell
cmake --version
```

If you have an older version, update CMake or use Solution 2 (manual download).

## Network/Firewall Issues

If the download fails due to network issues:
- Check your internet connection
- Try Solution 2 (manual download)
- Use a VPN if GitHub is blocked
- Download the ZIP manually and use Solution 3

