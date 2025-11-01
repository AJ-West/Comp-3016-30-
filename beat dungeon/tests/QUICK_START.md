# Quick Start Guide - Running Tests

## Step 1: Build the Tests

```powershell
cd "beat dungeon\tests"
if (Test-Path build) { Remove-Item -Recurse -Force build }
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
```

## Step 2: Run the Tests

### Option A: Run directly (Simple)
```powershell
.\Debug\BeatDungeonTests.exe
```

### Option B: Use CTest (Better output)
```powershell
ctest -C Debug --output-on-failure
```

### Option C: Run specific tests
```powershell
.\Debug\BeatDungeonTests.exe --gtest_filter="Base64Test.*"
.\Debug\BeatDungeonTests.exe --gtest_filter="PlayerObjectTest.*"
.\Debug\BeatDungeonTests.exe --gtest_filter="*Collision*"
```

## Step 3: Expected Output

You should see something like:
```
[==========] Running 86 tests from 10 test suites.
[----------] 6 tests from Base64Test
[ RUN      ] Base64Test.EncodeSimpleString
[       OK ] Base64Test.EncodeSimpleString (1 ms)
...
[==========] 86 tests from 10 test suites ran. (X ms total)
[  PASSED  ] 86 tests.
```

## From Visual Studio

1. Open the solution: `build/BeatDungeonTests.sln`
2. Set `BeatDungeonTests` as startup project
3. Press F5 to run, or right-click → Run Tests

## Troubleshooting

### "Cannot find BeatDungeonTests.exe"
- Make sure you built successfully (`cmake --build . --config Debug`)
- Check that you're in the `build` directory
- The executable should be in `build\Debug\BeatDungeonTests.exe`

### Tests fail immediately
- Check that SDL libraries are built (build main solution first)
- Check error messages for missing DLLs
- Some tests require SDL initialization - they'll skip if SDL fails

### Want verbose output?
```powershell
.\Debug\BeatDungeonTests.exe --gtest_color=yes
```

