# How to Run the Unit Tests

## Prerequisites

1. **CMake** (3.14 or higher)
   - Download from: https://cmake.org/download/
   - Or install via: `winget install Kitware.CMake` or `choco install cmake`

2. **C++ Compiler**
   - Visual Studio 2019/2022 with C++ development tools
   - Or MinGW/GCC on Windows

3. **Internet Connection** (for downloading Google Test)

## Method 1: Using the Batch Script (Easiest)

1. Open PowerShell or Command Prompt
2. Navigate to the tests directory:
   ```powershell
   cd "C:\Users\gamin\Documents\Final year uni\Comp-3016-30-\beat dungeon\tests"
   ```
3. Run the batch script:
   ```powershell
   .\run_tests.bat
   ```

**Note:** If you get an error about Visual Studio version, edit `run_tests.bat` and change:
- `"Visual Studio 17 2022"` to `"Visual Studio 16 2019"` (for VS 2019)
- Or `"Visual Studio 15 2017"` (for VS 2017)

## Method 2: Manual CMake Commands

### Step 1: Open PowerShell/Command Prompt
Navigate to the tests directory:
```powershell
cd "C:\Users\gamin\Documents\Final year uni\Comp-3016-30-\beat dungeon\tests"
```

### Step 2: Create Build Directory
```powershell
mkdir build
cd build
```

### Step 3: Configure CMake
For Visual Studio 2022 (x64):
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64
```

For Visual Studio 2019 (x64):
```powershell
cmake .. -G "Visual Studio 16 2019" -A x64
```

For Visual Studio 2017 (x64):
```powershell
cmake .. -G "Visual Studio 15 2017 Win64"
```

For MinGW:
```powershell
cmake .. -G "MinGW Makefiles"
```

### Step 4: Build the Tests
```powershell
cmake --build . --config Debug
```

### Step 5: Run the Tests

**Option A: Run directly**
```powershell
.\Debug\BeatDungeonTests.exe
```

**Option B: Use CTest (better output)**
```powershell
ctest -C Debug --output-on-failure
```

## Method 3: Using Visual Studio

1. Open Visual Studio
2. File → Open → CMake...
3. Navigate to and select: `beat dungeon/tests/CMakeLists.txt`
4. Visual Studio will detect CMake and configure automatically
5. Build → Build All (or press F7)
6. Test → Run All Tests (or press Ctrl+R, A)

## Expected Output

When tests run successfully, you should see output like:
```
[==========] Running 86 tests from 10 test suites.
[----------] Global test environment set-up.
[----------] 6 tests from Base64Test
[ RUN      ] Base64Test.EncodeSimpleString
[       OK ] Base64Test.EncodeSimpleString (1 ms)
...
[==========] 86 tests from 10 test suites ran. (X ms total)
[  PASSED  ] 86 tests.
```

## Troubleshooting

### Error: "CMake not found"
- Install CMake and make sure it's in your PATH
- Or use Visual Studio's built-in CMake support

### Error: "Cannot find SDL headers"
- Make sure the `dependencies` folder is in the correct location relative to `tests`
- Check that the paths in `CMakeLists.txt` are correct

### Error: "Visual Studio generator not found"
- Check your installed Visual Studio version
- Update the generator name in `run_tests.bat` or CMake command

### Error: "SDL_Init failed"
- Some tests require SDL initialization
- Tests that can't initialize SDL will skip automatically (this is expected)

### Tests fail to compile
- Make sure you have C++17 support enabled
- Check that all header files are accessible
- Verify include paths in CMakeLists.txt

## Running Specific Tests

To run only specific tests, use filters:
```powershell
.\Debug\BeatDungeonTests.exe --gtest_filter="Base64Test.*"
.\Debug\BeatDungeonTests.exe --gtest_filter="PlayerObjectTest.*"
.\Debug\BeatDungeonTests.exe --gtest_filter="*Collision*"
```

## Cleaning Build Files

To clean and rebuild:
```powershell
cd build
rmdir /s /q *
cmake ..
cmake --build . --config Debug
```

## Quick Reference

```powershell
# Full rebuild and test
cd "beat dungeon\tests"
if (Test-Path build) { Remove-Item -Recurse -Force build }
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
.\Debug\BeatDungeonTests.exe
```

