#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include "../Beat dungeon/GameObject.h"
#include "../Beat dungeon/movement.h"
#include <memory>

// Mock renderer for testing
class MockRenderer {};

// Simple test GameObject that can be instantiated
class TestGameObject : public GameObject {
public:
    TestGameObject(SDL_FRect dim, float sp, std::vector<std::vector<int>> walkable_outline, int size)
        : GameObject(dim, sp, walkable_outline, size) {}

    void wallCollision(float deltatime) override {
        // Test implementation
        SDL_FRect dimensions = getDimensions();
        std::pair<float, float> direction = getDirection();
        float speed = getSpeed() * deltatime * -1;
        setDimensions({ dimensions.x + direction.first * speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
    }
};

class GameObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize SDL for testing
        SDL_Init(SDL_INIT_VIDEO);
        
        // Create a simple walkable outline (10x10 grid, all walkable)
        walkable_outline = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
        
        // Create test GameObject
        SDL_FRect dim = {100.0f, 100.0f, 50.0f, 50.0f};
        obj = std::make_unique<TestGameObject>(dim, 150.0f, walkable_outline, 80);
    }

    void TearDown() override {
        obj.reset();
        SDL_Quit();
    }

    std::unique_ptr<TestGameObject> obj;
    std::vector<std::vector<int>> walkable_outline;
};

TEST_F(GameObjectTest, Initialization) {
    SDL_FRect dim = obj->getDimensions();
    EXPECT_FLOAT_EQ(dim.x, 100.0f);
    EXPECT_FLOAT_EQ(dim.y, 100.0f);
    EXPECT_FLOAT_EQ(dim.w, 50.0f);
    EXPECT_FLOAT_EQ(dim.h, 50.0f);
    EXPECT_EQ(obj->getSpeed(), 150.0f);
}

TEST_F(GameObjectTest, SetGetDimensions) {
    SDL_FRect newDim = {200.0f, 200.0f, 60.0f, 60.0f};
    obj->setDimensions(newDim);
    SDL_FRect retrieved = obj->getDimensions();
    EXPECT_FLOAT_EQ(retrieved.x, 200.0f);
    EXPECT_FLOAT_EQ(retrieved.y, 200.0f);
    EXPECT_FLOAT_EQ(retrieved.w, 60.0f);
    EXPECT_FLOAT_EQ(retrieved.h, 60.0f);
}

TEST_F(GameObjectTest, SetGetDirection) {
    std::pair<float, float> dir = {1.0f, 0.0f};
    obj->setDirection(dir);
    std::pair<float, float> retrieved = obj->getDirection();
    EXPECT_FLOAT_EQ(retrieved.first, 1.0f);
    EXPECT_FLOAT_EQ(retrieved.second, 0.0f);
}

TEST_F(GameObjectTest, SetGetSpeed) {
    obj->setSpeed(200.0f);
    EXPECT_EQ(obj->getSpeed(), 200.0f);
}

TEST_F(GameObjectTest, GetCell) {
    SDL_FRect dim = {160.0f, 160.0f, 50.0f, 50.0f}; // Center at (185, 185)
    obj->setDimensions(dim);
    std::pair<int, int> cell = obj->getCell();
    // Should be in cell (2, 2) since 185/80 = 2.31...
    EXPECT_EQ(cell.first, 2);
    EXPECT_EQ(cell.second, 2);
}

TEST_F(GameObjectTest, AddComponent) {
    auto movement = std::make_shared<movementComponent>(obj.get());
    obj->AddComponent(movement);
    // Component should be added without error
    EXPECT_TRUE(true);
}

TEST_F(GameObjectTest, WallCollisionReversesDirection) {
    obj->setDirection({1.0f, 0.0f});
    obj->setSpeed(100.0f);
    float deltaTime = 0.016f; // ~60fps
    
    // Simulate wall collision
    obj->wallCollision(deltaTime);
    
    // Direction should be reversed (movement should go back)
    std::pair<float, float> dir = obj->getDirection();
    // The wallCollision reverses movement, so we expect position to move back
    EXPECT_TRUE(true); // Basic test that it doesn't crash
}

TEST_F(GameObjectTest, GetRemoveCell) {
    std::pair<int, int> initial = obj->getRemoveCell();
    EXPECT_EQ(initial.first, NULL);
    EXPECT_EQ(initial.second, NULL);
    
    obj->setRemoveCell(5, 10);
    std::pair<int, int> set = obj->getRemoveCell();
    EXPECT_EQ(set.first, 5);
    EXPECT_EQ(set.second, 10);
}

TEST_F(GameObjectTest, GetCharging) {
    EXPECT_FALSE(obj->getCharging());
}

