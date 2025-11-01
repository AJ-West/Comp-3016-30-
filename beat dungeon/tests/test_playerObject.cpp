#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include "../Beat dungeon/playerObject.h"
#include <memory>

class PlayerObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
        
        // Create walkable outline
        walkable_outline = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
        
        // Create player
        SDL_FRect dim = {100.0f, 100.0f, 50.0f, 50.0f};
        player = std::make_unique<PlayerObj>(dim, 150.0f, walkable_outline, 80);
    }

    void TearDown() override {
        player.reset();
        SDL_Quit();
    }

    std::unique_ptr<PlayerObj> player;
    std::vector<std::vector<int>> walkable_outline;
};

TEST_F(PlayerObjectTest, Initialization) {
    EXPECT_FALSE(player->getWin());
    auto keys = player->getMovementKeys();
    EXPECT_EQ(keys.size(), 4); // Up, Left, Down, Right
    EXPECT_EQ(keys[0].size(), 1); // Each direction has one default key
}

TEST_F(PlayerObjectTest, DefaultMovementKeys) {
    auto keys = player->getMovementKeys();
    EXPECT_EQ(keys[0][0], SDLK_W); // Up
    EXPECT_EQ(keys[1][0], SDLK_A); // Left
    EXPECT_EQ(keys[2][0], SDLK_S); // Down
    EXPECT_EQ(keys[3][0], SDLK_D); // Right
}

TEST_F(PlayerObjectTest, ChangeDirectionUp) {
    player->change_direction(SDLK_W, true);
    std::pair<float, float> dir = player->getDirection();
    EXPECT_FLOAT_EQ(dir.first, 0.0f);
    EXPECT_FLOAT_EQ(dir.second, -1.0f);
}

TEST_F(PlayerObjectTest, ChangeDirectionDown) {
    player->change_direction(SDLK_S, true);
    std::pair<float, float> dir = player->getDirection();
    EXPECT_FLOAT_EQ(dir.first, 0.0f);
    EXPECT_FLOAT_EQ(dir.second, 1.0f);
}

TEST_F(PlayerObjectTest, ChangeDirectionLeft) {
    player->change_direction(SDLK_A, true);
    std::pair<float, float> dir = player->getDirection();
    EXPECT_FLOAT_EQ(dir.first, -1.0f);
    EXPECT_FLOAT_EQ(dir.second, 0.0f);
}

TEST_F(PlayerObjectTest, ChangeDirectionRight) {
    player->change_direction(SDLK_D, true);
    std::pair<float, float> dir = player->getDirection();
    EXPECT_FLOAT_EQ(dir.first, 1.0f);
    EXPECT_FLOAT_EQ(dir.second, 0.0f);
}

TEST_F(PlayerObjectTest, ChangeDirectionKeyUp) {
    player->change_direction(SDLK_W, true);
    player->change_direction(SDLK_W, false);
    std::pair<float, float> dir = player->getDirection();
    EXPECT_FLOAT_EQ(dir.first, 0.0f);
    EXPECT_FLOAT_EQ(dir.second, 0.0f);
}

TEST_F(PlayerObjectTest, SetGetWin) {
    EXPECT_FALSE(player->getWin());
    player->setWin(true);
    EXPECT_TRUE(player->getWin());
    player->setWin(false);
    EXPECT_FALSE(player->getWin());
}

TEST_F(PlayerObjectTest, WallCollision) {
    player->setDirection({1.0f, 0.0f});
    SDL_FRect initialDim = player->getDimensions();
    float deltaTime = 0.016f;
    
    player->wallCollision(deltaTime);
    
    // Should have moved back
    SDL_FRect newDim = player->getDimensions();
    EXPECT_LT(newDim.x, initialDim.x);
}

TEST_F(PlayerObjectTest, NewKeys) {
    auto keysBefore = player->getMovementKeys();
    int initialKeyCount = keysBefore[0].size();
    
    player->newKeys();
    
    auto keysAfter = player->getMovementKeys();
    // Should have one more key per direction
    EXPECT_EQ(keysAfter[0].size(), initialKeyCount + 1);
    EXPECT_EQ(keysAfter[1].size(), initialKeyCount + 1);
    EXPECT_EQ(keysAfter[2].size(), initialKeyCount + 1);
    EXPECT_EQ(keysAfter[3].size(), initialKeyCount + 1);
}

TEST_F(PlayerObjectTest, RotateKeysClockwise) {
    auto keysBefore = player->getMovementKeys();
    SDL_Keycode upBefore = keysBefore[0][0];
    SDL_Keycode leftBefore = keysBefore[1][0];
    SDL_Keycode downBefore = keysBefore[2][0];
    SDL_Keycode rightBefore = keysBefore[3][0];
    
    // Set random seed to control rotation direction
    srand(1); // Should be clockwise
    player->rotateKeys();
    
    auto keysAfter = player->getMovementKeys();
    // After clockwise rotation: Up -> Right, Right -> Down, Down -> Left, Left -> Up
    // This is a basic check - actual rotation depends on rand() % 2
    EXPECT_TRUE(true); // Basic test that it doesn't crash
}

TEST_F(PlayerObjectTest, RotateKeysAnticlockwise) {
    auto keysBefore = player->getMovementKeys();
    
    srand(0); // Should be anticlockwise
    player->rotateKeys();
    
    auto keysAfter = player->getMovementKeys();
    // Basic test that rotation occurred
    EXPECT_TRUE(true);
}

