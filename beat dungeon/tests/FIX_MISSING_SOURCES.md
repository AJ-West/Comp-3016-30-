# Fixing Missing Source Files Errors

The linker errors show that `KeyTime` and `Button` classes are not being compiled. I've added the source files to CMakeLists.txt, but you may need to rebuild.

## The Issue

The tests include headers (`KeyTime.h`, `button.h`) but the linker can't find the implementations (`KeyTime.cpp`, `button.cpp`). 

## Solution: Rebuild After CMake Update

1. **Clean the build directory**:
   ```powershell
   cd "beat dungeon\tests\build"
   Remove-Item -Recurse -Force *
   ```

2. **Reconfigure CMake**:
   ```powershell
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

3. **Check CMake output** - You should see:
   - "Found SDL_ttf: ..." (if library exists)
   - Source files being added to the project

4. **Build again**:
   ```powershell
   cmake --build . --config Debug
   ```

## If SDL_ttf Library Still Not Found

The tests need SDL_ttf library. Build it first:

1. **Open the main solution**:
   ```
   beat dungeon/Beat dungeon/Beat dungeon.sln
   ```

2. **Build SDL_ttf project**:
   - Right-click on SDL_ttf project → Build
   - Or build the entire solution

3. **Verify library exists**:
   - Check: `dependencies/SDL_ttf/VisualC/SDL_ttf/x64/Debug/SDL3_ttf.lib`
   - Or: `dependencies/SDL_ttf/VisualC/SDL_ttf/x64/Debug/SDL_ttf.lib`

4. **Reconfigure CMake** to find the newly built library

## Alternative: Skip TTF-Dependent Tests

If you can't build SDL_ttf, you can temporarily comment out TTF tests:

1. In `CMakeLists.txt`, comment out:
   ```cmake
   # test_keyTime.cpp
   ```

2. Rebuild

## Verify Source Files Are Included

After reconfiguring, check the generated Visual Studio project:
- Open `build/BeatDungeonTests.vcxproj`
- Look for `KeyTime.cpp`, `button.cpp`, `globals.cpp` in the `<ClCompile>` section

If they're missing, CMake didn't pick them up - check the paths in CMakeLists.txt.

