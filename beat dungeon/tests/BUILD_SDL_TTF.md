# Building SDL_ttf Library

The tests require SDL_ttf library to be built first. Here's how:

## Quick Steps

1. **Open SDL_ttf solution**:
   ```
   dependencies/SDL_ttf/VisualC/SDL_ttf.sln
   ```

2. **Set configuration to x64 Debug** (or Release)

3. **Build the solution** (Build → Build Solution or F7)

4. **Verify library was created**:
   Check: `dependencies/SDL_ttf/VisualC/x64/Debug/SDL3_ttf.lib`
   Or: `dependencies/SDL_ttf/VisualC/x64/Debug/SDL_ttf.lib`

5. **Reconfigure CMake**:
   ```powershell
   cd "beat dungeon\tests\build"
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

6. **Rebuild tests**:
   ```powershell
   cmake --build . --config Debug
   ```

## Alternative: Build from Main Solution

If your main game solution includes SDL_ttf as a project reference:

1. Open: `beat dungeon/Beat dungeon/Beat dungeon.sln`
2. Build the solution (this should build SDL_ttf automatically)
3. The library should be in: `dependencies/SDL_ttf/VisualC/x64/Debug/`

## Verify Library Exists

Run this PowerShell command to check:
```powershell
Get-ChildItem -Path "dependencies\SDL_ttf\VisualC\x64\Debug" -Filter "*.lib"
```

You should see `SDL3_ttf.lib` or `SDL_ttf.lib`.

## If Library Still Not Found

After building, check the CMake output when you reconfigure. It should show:
```
Found SDL_ttf: C:/.../SDL_ttf/VisualC/x64/Debug/SDL3_ttf.lib
```

If it doesn't find it, the library might be in a different location. Check the Visual Studio Output window during build to see where it's placing the .lib file.

