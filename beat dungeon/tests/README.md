# Beat Dungeon Unit Tests

This directory contains comprehensive unit tests for the Beat Dungeon game project.

## Setup

### Prerequisites
- CMake 3.14 or higher
- C++17 compatible compiler
- Google Test (automatically downloaded via CMake)

### Building Tests

```bash
cd "beat dungeon/tests"
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
# Run all tests
./BeatDungeonTests

# Or with CMake
ctest
```

## Test Coverage

### Test Files

1. **test_base64.cpp** - Tests for base64 encoding/decoding functions
   - Encode/decode simple strings
   - Encode/decode empty strings
   - Round-trip encoding verification
   - Known value validation

2. **test_gameObject.cpp** - Tests for GameObject base class
   - Initialization
   - Dimension getters/setters
   - Direction getters/setters
   - Speed management
   - Cell position calculation
   - Component system
   - Wall collision

3. **test_playerObject.cpp** - Tests for PlayerObj class
   - Initialization and default keys
   - Direction changes (WASD)
   - Win state management
   - Wall collision
   - New keys powerup
   - Key rotation

4. **test_monsterObject.cpp** - Tests for MonsterObj class
   - Initialization and stun state
   - Hit detection
   - Charging state
   - Wall collision
   - Stun timer

5. **test_pathfinding.cpp** - Tests for A* pathfinding algorithm
   - Node creation and comparison
   - Pathfinding component creation
   - Simple pathfinding scenarios
   - Pathfinding with obstacles
   - No path scenarios

6. **test_collision.cpp** - Tests for collision detection
   - Player collision detection
   - Charge collision detection
   - Environment collision (walls, trapdoors, powerups)
   - Line of sight checks
   - Range-based collision

7. **test_keyTime.cpp** - Tests for KeyTime class
   - Key creation (good/bad keys)
   - Usage tracking
   - Progress calculation
   - Zone detection
   - Expiration timing

8. **test_button.cpp** - Tests for Button class
   - Button creation
   - Hover detection
   - Click handling
   - Function callbacks
   - Level loading callbacks
   - Boundary conditions

9. **test_particles.cpp** - Tests for particle system
   - Particle initialization
   - Particle update and lifetime
   - Particle pool management
   - Color variations
   - Fade out effects

## Test Structure

Each test file follows Google Test conventions:
- `TEST_F` for fixture-based tests
- `SetUp()` and `TearDown()` for initialization/cleanup
- Descriptive test names following `ClassTest, FunctionTest` pattern

## Integration with Visual Studio

To integrate these tests with your Visual Studio project:

1. Right-click on the solution
2. Add → Existing Project
3. Navigate to `tests/` directory
4. Select the CMakeLists.txt or create a new test project

Alternatively, you can use Visual Studio's built-in test explorer if you configure CMake integration.

## Notes

- Some tests require SDL initialization and may skip if renderer creation fails
- Thread-based tests (like monster stun timer) may have timing dependencies
- Random-based tests (like key rotation) may have non-deterministic behavior
- Tests are designed to not modify existing source files

## Contributing

When adding new features, please add corresponding unit tests to ensure code quality and prevent regressions.

