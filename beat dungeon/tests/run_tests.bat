@echo off
REM Script to build and run tests on Windows

echo Building Beat Dungeon Tests...

REM Create build directory
if not exist build mkdir build
cd build

REM Configure CMake (adjust generator if needed)
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build
cmake --build . --config Debug

REM Run tests
echo.
echo Running tests...
Debug\BeatDungeonTests.exe

REM Run with CTest for better output
echo.
echo Running tests with CTest...
ctest -C Debug --output-on-failure

pause

