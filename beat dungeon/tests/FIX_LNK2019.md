# Fixing LNK2019 Unresolved External Symbol Errors

LNK2019 errors occur when the linker can't find required libraries. The tests need to link against:
- SDL3
- SDL3_image  
- SDL3_ttf
- irrKlang

## Solution 1: Build SDL Libraries First (Recommended)

Since your project uses Visual Studio project references, you need to build SDL libraries first:

1. **Open the main solution**:
   ```
   beat dungeon/Beat dungeon/Beat dungeon.sln
   ```

2. **Build the solution** in Visual Studio (this builds SDL, SDL_image, SDL_ttf)

3. **Find the built libraries** - they should be in:
   - `dependencies/SDL/VisualC/SDL/x64/Debug/SDL3.lib` (or Release)
   - `dependencies/SDL_image-release-3.2.4/VisualC/SDL_image/x64/Debug/SDL3_image.lib`
   - `dependencies/SDL_ttf/VisualC/SDL_ttf/x64/Debug/SDL3_ttf.lib`

4. **Rebuild the tests** - CMake should now find these libraries

## Solution 2: Manual Library Paths

If CMake can't find the libraries automatically, edit `CMakeLists.txt` and add explicit paths:

```cmake
# Add after the library finding section
set(SDL3_LIBRARY "${CMAKE_SOURCE_DIR}/../dependencies/SDL/VisualC/SDL/x64/Debug/SDL3.lib")
set(SDL3_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/../dependencies/SDL_image-release-3.2.4/VisualC/SDL_image/x64/Debug/SDL3_image.lib")
set(SDL3_TTF_LIBRARY "${CMAKE_SOURCE_DIR}/../dependencies/SDL_ttf/VisualC/SDL_ttf/x64/Debug/SDL3_ttf.lib")
set(IRRKLANG_LIBRARY "${CMAKE_SOURCE_DIR}/../dependencies/irrKlang/lib/Winx64-visualStudio/irrKlang.lib")
```

## Solution 3: Use SDL CMake Build

If SDL has CMakeLists.txt, you can build it with CMake:

```powershell
cd "dependencies\SDL"
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
```

Then update CMakeLists.txt to use the CMake-built SDL.

## Solution 4: Skip SDL-Dependent Tests

If you just want to test non-SDL code, you can comment out SDL-dependent tests:

- `test_keyTime.cpp` (uses SDL/TTF)
- `test_button.cpp` (uses SDL/Image)
- Some tests in `test_gameObject.cpp`, `test_playerObject.cpp`, etc. that initialize SDL

Then rebuild.

## Quick Diagnostic

Run CMake with verbose output to see what it finds:

```powershell
cd tests\build
cmake .. -G "Visual Studio 17 2022" -A x64 --debug-output
```

This will show you what libraries CMake is (or isn't) finding.

## Common Issues

### "Cannot find SDL3.lib"
- Build the main solution first to generate SDL libraries
- Check the library paths match your build configuration (Debug vs Release)

### "Cannot find irrKlang.lib"
- Verify `dependencies/irrKlang/lib/Winx64-visualStudio/irrKlang.lib` exists
- If using x86, change path to `Win32-visualStudio`

### "Multiple definitions"
- Make sure you're linking libraries, not including source files
- Check that you're not including `.cpp` files from the game in your test executable

## Verify Library Locations

Check these paths exist:
- ✅ `dependencies/SDL/VisualC/SDL/x64/Debug/SDL3.lib`
- ✅ `dependencies/SDL_image-release-3.2.4/VisualC/SDL_image/x64/Debug/SDL3_image.lib`  
- ✅ `dependencies/SDL_ttf/VisualC/SDL_ttf/x64/Debug/SDL3_ttf.lib`
- ✅ `dependencies/irrKlang/lib/Winx64-visualStudio/irrKlang.lib`

If any are missing, build the main solution first!

