# Beat dungeon

## Project overview

"Beat dungeon" is a small 2D tile-based prototype game created as a final-year project. The player controls a Knight that moves through dungeon levels, collecting and rotating movement keys, avoiding monsters (minotaur, skeleton), using powerups (speed boost, new keys, rotate-keys) and reaching trapdoors / exits.

This repository contains the game source (Visual Studio solution and C++ source), dependent third-party libraries, assets (Aseprite files) and a unit test suite built with GoogleTest.

## Gameplay description

- Top-down 2D dungeon with discrete tile grid (cell size ~80px). The player and monsters move in the grid and interact with special tiles.
- Player movement is driven by configurable movement key groups (up/left/down/right). Powerups on the map may spawn additional movement keys or rotate existing key groups.
- Monsters use simple AI: they chase or charge the player (chargeCollision), can stun on wall collisions, and spawn particle effects on events.
- Environment tiles include walls, trapdoors (win condition), speed-boost tiles (double player speed temporarily) and keys-related tiles.

Controls: WASD by default (configurable in code via PlayerObj movement keys).

## Dependencies used

- SDL3 (windowing, input, basic rendering)
- SDL3_image (image loading)
- SDL3_ttf (text/font support; harfbuzz used inside ttf)
- irrKlang (optional audio support — provided under `dependencies/irrKlang`)
- GoogleTest (unit tests under `beat dungeon/tests`)

On Windows you will also need some system import libraries at link time (these are part of the OS SDK):
- `Rpcrt4.lib` (UuidCreate)
- `usp10.lib` (Uniscribe: ScriptItemize / ScriptShape / ScriptPlace / ScriptFreeCache)

If building with CMake, add those libraries to your target_link_libraries line for the EXE or library that links SDL_ttf.

## Use of AI

This project was developed by the student; the game itself does not embed machine learning or AI models. If this repository was assisted by automated tools (code helpers, linters or automated edits), list them here. If you (the author) used any automated assistants during development, add a short note explaining what they helped with (for example: automated test harness, CMake fixes, small bug patches to make unit tests pass).

Template (fill in if applicable):
- Tools / assistants used: [e.g. ChatGPT, Copilot, clang-tidy]
- Tasks done by assistant: [e.g. test CMake fixes, small source fixes, PR suggestions]

## Game programming patterns used

- Component-based GameObject: `GameObject` holds a collection of `Component`s (e.g., collision, particle, pathfinding) and forwards Update calls.
- Tile-based world: a 2D grid (walkable outline) describes map tiles and special tile effects.
- Object-oriented polymorphism: `PlayerObj` and `MonsterObj` inherit from `GameObject` and override behaviour such as `wallCollision`.
- Particle pool: reusable particle instances to avoid frequent allocations and to implement particle effects efficiently.
- Pathfinding: A* / neighbor graph implemented under `pathfinding.h` and used by pathfinding component.
- Component-driven collision and behaviour separation: collision responsibilities are in `playerCollision.h`, `chargeCollision.h`, `wallCollision.h`.

## Game mechanics and how they are coded (high-level)

- Movement: Each GameObject stores a direction pair and speed. On Update, positions are advanced by direction * speed * deltaTime.
- Collisions:
	- Player collision detection with monsters handled by `playerCollisionComponent`.
	- Charge behaviour is implemented in `chargeCollisionComponent` which checks distance/line-of-sight and triggers charged movement.
	- Environment collisions (tile effects) are handled by `environmentCollisionComponent` in `wallCollision.h` and call into the owner GameObject to apply effects (setSpeed, setWin, newKeys, rotateKeys).
- Wall collision response reverses or adjusts direction and may stun monsters (MonsterObj implements a stun timer and speed changes).
- Powerups are encoded as tile values inside the `walkable_outline` 2D vector; the `environmentCollisionComponent` reads the tile under the object's cell and applies effects.
- Keys: Player maintains movement key groups (vectors of SDL_Keycode). `newKeys()` appends a random key from a potential pool and `rotateKeys()` rotates the groups.
- Particles: The particle system spawns small temporary sprites with lifetime and color, updated by a particle pool.

## UML / Design diagram

I haven't included a formal UML image in the repo. Below is a compact textual class diagram to help navigation — you can paste this into an online UML tool or draw it in a diagram editor.

- GameObject
	- +dimensions: SDL_FRect
	- +speed: float
	- +components: map<string, shared_ptr<Component>>
	- +Update(delta)
	- +wallCollision(delta) (virtual)

- Component (abstract)
	- +update(delta) (pure virtual)
	- +owner: GameObject*

- PlayerObj : GameObject
	- +movement_keys: vector<vector<SDL_Keycode>>
	- +newKeys(), rotateKeys(), change_direction()

- MonsterObj : GameObject
	- +stunned, hit, charging
	- +stun(), wallCollision() overrides

- environmentCollisionComponent : Component
	- reads walkable_outline (map tiles) and applies tile effects (setSpeed, setWin, newKeys, rotateKeys)

If you want I can generate a PlantUML diagram from these classes and add it to the repo.

## Sample screens / assets

The repository contains Aseprite source files under `Beat dungeon/images/` for the art used by the prototype, including:
- `Knight.aseprite`, `minotaur.aseprite`, `skeleton.aseprite`, `walls.aseprite`, level screens and UI frames.

Add rendered PNGs here (example placeholders):
- docs/screens/main_menu.png (placeholder)
- docs/screens/level1.png (placeholder)

If you provide exported screenshots I can add them to `docs/screens/` and update this README to include thumbnails.

## Exception handling and test cases

- Basic defensive checks and guards occur in places such as bounds-checking of tile indices in `environmentCollisionComponent::update` to avoid out-of-range access.
- There is a GoogleTest-based unit test suite in `beat dungeon/tests/`. It covers:
	- GameObject basics (dimensions, direction, speed, component management)
	- PlayerObj behaviour (movement keys, key rotation, wall collision)
	- MonsterObj behaviour (stun, collision, particle spawning)
	- Pathfinding routines and neighbor-finding
	- Collision components (player, charge, environment)
	- Particle lifecycle and pool behaviour

- Some tests are renderer-dependent and will be skipped if an SDL renderer cannot be created in the test environment.

How to run tests (Windows, PowerShell):

```powershell
cd "beat dungeon/tests"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
& "$(Resolve-Path build\\Debug\\BeatDungeonTests.exe)" --gtest_color=no
```

Notes: when linking on Windows you may need to add system import libraries to the target (see Dependencies used). If tests fail due to missing DLLs, copy the SDL/SDL_image/SDL_ttf runtime DLLs from `Beat dungeon/x64/Debug` next to the test exe or add the bin folder to PATH.

## Further technical details (how the prototype works)

- Map representation: `walkable_outline` is a 2D vector<int> where integer values encode tile types (0 = empty/walkable, 1 = wall, 2 = trapdoor, 3 = speed modifier, 4 = new keys, 5 = rotate keys).
- Objects compute their current cell by integer-dividing their center coordinates by `cell_size`.
- Components are attached to GameObjects at runtime using `AddComponent` and updated in the `GameObject::Update` loop.
- The codebase mixes header-only components and inline logic for small systems; tests compile the implementation files directly in the test target for convenience.

## Build notes / known platform issues

- On Windows linking with the prebuilt `SDL3_ttf.lib` may require additional system import libraries:
	- `Rpcrt4.lib` (resolves UuidCreate used by HarfBuzz inside SDL_ttf)
	- `usp10.lib` (Uniscribe functions used by HarfBuzz)

- If you see other unresolved externals, add the appropriate import libs (Bcrypt.lib, Ole32.lib, Ws2_32.lib, Gdi32.lib) depending on the missing symbols.

## Tests: status and how I validated fixes

- A unit test run (local dev) currently shows a majority of tests passing; a few were fixed by small source changes to `GameObject`, `monsterObject`, `button.cpp`, and `wallCollision.h` to ensure correct semantics and avoid crashes.
- Tests are useful to validate behavior of wall collisions, particle lifetimes, key rotation and other deterministic systems. Tests that rely on an SDL renderer may be skipped in headless CI.

## Short evaluation — what I achieved and what I'd change

What I achieved:
- A working prototype with player and monster behaviors, tile-based powerups, particle effects, and a unit test suite that covers most non-renderer logic.
- Automated test CMake glue to build tests together with the game's implementation files so unit tests can run without a separate library build step.

What I'd do differently / next steps:
- Build the game code as a proper static library (or set of libraries) and link the test target against that instead of re-globbing/sourcing .cpp into the test target. This improves modularity and compile times.
- Add a minimal headless renderer or a renderer abstraction that can be stubbed for unit tests so no tests need to skip when a real GPU/renderer isn't available.
- Improve error handling and replace ad-hoc cout fallbacks (e.g., GameObject::setWin prints "win" by default) with proper logging and testable behavior.
- Add CI configuration (GitHub Actions) that builds and runs tests on Windows and Linux and uploads test artifacts/screenshots.

If you want I can:
- Generate a PlantUML file describing the core classes and add an SVG to `docs/`.
- Add a simple `docs/screens/` folder and copy a few in-repo exported screenshots into it.
- Patch the CMakeLists to link `Rpcrt4` and `usp10` automatically so Visual Studio linking issues are avoided.

## Contact / next steps

Tell me which of the optional follow-ups you'd like me to do now and I will implement them (PlantUML, screenshot insertion, CMake patch to add system libs, or continue fixing the remaining failing tests).

# Comp-3016-30-
AJ West's work for the comp 3016 30% coursework
