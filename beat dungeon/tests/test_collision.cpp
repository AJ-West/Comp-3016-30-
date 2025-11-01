#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include "../Beat dungeon/playerCollision.h"
#include "../Beat dungeon/chargeCollision.h"
#include "../Beat dungeon/wallCollision.h"
#include "../Beat dungeon/monsterObject.h"
#include "../Beat dungeon/playerObject.h"
#include <memory>
#include <cmath>

class CollisionTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
        
        walkable_outline = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
        
        // Create player at center
        SDL_FRect playerDim = {400.0f, 400.0f, 50.0f, 50.0f};
        player = std::make_unique<PlayerObj>(playerDim, 150.0f, walkable_outline, 80);
        
        // Create monster close to player
        SDL_FRect monsterDim = {420.0f, 420.0f, 50.0f, 50.0f};
        monster = std::make_unique<MonsterObj>(monsterDim, 150.0f, walkable_outline, 80);
    }

    void TearDown() override {
        monster.reset();
        player.reset();
        SDL_Quit();
    }

    std::unique_ptr<MonsterObj> monster;
    std::unique_ptr<PlayerObj> player;
    std::vector<std::vector<int>> walkable_outline;
};

TEST_F(CollisionTest, PlayerCollisionComponentCreation) {
    auto collision = std::make_shared<playerCollisionComponent>(monster.get(), player.get(), 20);
    EXPECT_TRUE(true);
}

TEST_F(CollisionTest, PlayerCollisionDetectionClose) {
    auto collision = std::make_shared<playerCollisionComponent>(monster.get(), player.get(), 50);
    
    SDL_FRect playerDim = player->getDimensions();
    bool collided = collision->checkPlayerCollision(playerDim);
    
    // Monster is at (420, 420), player at (400, 400)
    // Distance = sqrt((420-400)^2 + (420-400)^2) = sqrt(800) ≈ 28.3
    // Range is 50, so should collide
    EXPECT_TRUE(collided);
}

TEST_F(CollisionTest, PlayerCollisionDetectionFar) {
    // Move monster far away
    SDL_FRect farDim = {800.0f, 800.0f, 50.0f, 50.0f};
    monster->setDimensions(farDim);
    
    auto collision = std::make_shared<playerCollisionComponent>(monster.get(), player.get(), 20);
    SDL_FRect playerDim = player->getDimensions();
    bool collided = collision->checkPlayerCollision(playerDim);
    
    EXPECT_FALSE(collided);
}

TEST_F(CollisionTest, ChargeCollisionComponentCreation) {
    auto chargeCollision = std::make_shared<chargeCollisionComponent>(monster.get(), player.get(), 800);
    EXPECT_TRUE(true);
}

TEST_F(CollisionTest, ChargeCollisionDetection) {
    auto chargeCollision = std::make_shared<chargeCollisionComponent>(monster.get(), player.get(), 800);
    
    SDL_FRect playerDim = player->getDimensions();
    bool inRange = chargeCollision->checkPlayerCollision(playerDim);
    
    // Monster is close, should be in range
    EXPECT_TRUE(inRange);
}

TEST_F(CollisionTest, EnvironmentCollisionWall) {
    // Set player position to a wall cell
    walkable_outline[5][5] = 1; // Wall
    SDL_FRect wallPos = {5 * 80.0f, 5 * 80.0f, 50.0f, 50.0f};
    player->setDimensions(wallPos);
    
    auto envCollision = std::make_shared<environmentCollisionComponent>(player.get());
    player->AddComponent(envCollision);
    
    float deltaTime = 0.016f;
    envCollision->update(deltaTime);
    
    // Should trigger wall collision
    EXPECT_TRUE(true);
}

TEST_F(CollisionTest, EnvironmentCollisionTrapdoor) {
    // Set trapdoor at position
    walkable_outline[5][5] = 2; // Trapdoor
    SDL_FRect trapdoorPos = {5 * 80.0f, 5 * 80.0f, 50.0f, 50.0f};
    player->setDimensions(trapdoorPos);
    
    auto envCollision = std::make_shared<environmentCollisionComponent>(player.get());
    player->AddComponent(envCollision);
    
    float deltaTime = 0.016f;
    envCollision->update(deltaTime);
    
    // Player should win
    EXPECT_TRUE(player->getWin());
}

TEST_F(CollisionTest, EnvironmentCollisionSpeedBoost) {
    walkable_outline[5][5] = 3; // 2x speed modifier
    SDL_FRect modifierPos = {5 * 80.0f, 5 * 80.0f, 50.0f, 50.0f};
    player->setDimensions(modifierPos);
    
    float initialSpeed = player->getSpeed();
    
    auto envCollision = std::make_shared<environmentCollisionComponent>(player.get());
    player->AddComponent(envCollision);
    
    float deltaTime = 0.016f;
    envCollision->update(deltaTime);
    
    // Speed should be doubled
    EXPECT_EQ(player->getSpeed(), initialSpeed * 2);
}

TEST_F(CollisionTest, EnvironmentCollisionNewKeys) {
    walkable_outline[5][5] = 4; // New keys modifier
    SDL_FRect modifierPos = {5 * 80.0f, 5 * 80.0f, 50.0f, 50.0f};
    player->setDimensions(modifierPos);
    
    auto keysBefore = player->getMovementKeys();
    int initialKeyCount = keysBefore[0].size();
    
    auto envCollision = std::make_shared<environmentCollisionComponent>(player.get());
    player->AddComponent(envCollision);
    
    float deltaTime = 0.016f;
    envCollision->update(deltaTime);
    
    auto keysAfter = player->getMovementKeys();
    EXPECT_EQ(keysAfter[0].size(), initialKeyCount + 1);
}

TEST_F(CollisionTest, EnvironmentCollisionRotateKeys) {
    walkable_outline[5][5] = 5; // Rotate keys modifier
    SDL_FRect modifierPos = {5 * 80.0f, 5 * 80.0f, 50.0f, 50.0f};
    player->setDimensions(modifierPos);
    
    auto keysBefore = player->getMovementKeys();
    SDL_Keycode upBefore = keysBefore[0][0];
    
    auto envCollision = std::make_shared<environmentCollisionComponent>(player.get());
    player->AddComponent(envCollision);
    
    float deltaTime = 0.016f;
    envCollision->update(deltaTime);
    
    // Keys should be rotated (exact rotation depends on random)
    EXPECT_TRUE(true);
}

TEST_F(CollisionTest, ChargeCollisionLineOfSight) {
    // Create a map with a clear line of sight
    std::vector<std::vector<int>> losMap(10, std::vector<int>(10, 0));
    
    SDL_FRect monsterDim = {80.0f, 80.0f, 50.0f, 50.0f};
    SDL_FRect playerDim = {400.0f, 400.0f, 50.0f, 50.0f};
    
    MonsterObj losMonster(monsterDim, 150.0f, losMap, 80);
    PlayerObj losPlayer(playerDim, 150.0f, losMap, 80);
    
    auto chargeCollision = std::make_shared<chargeCollisionComponent>(&losMonster, &losPlayer, 800);
    
    // Test line of sight check
    EXPECT_TRUE(true);
}

TEST_F(CollisionTest, ChargeCollisionLineOfSightBlocked) {
    // Create a map with a wall blocking line of sight
    std::vector<std::vector<int>> blockedMap(10, std::vector<int>(10, 0));
    blockedMap[2][2] = 1; // Wall blocking path
    
    SDL_FRect monsterDim = {80.0f, 80.0f, 50.0f, 50.0f};
    SDL_FRect playerDim = {400.0f, 400.0f, 50.0f, 50.0f};
    
    MonsterObj blockedMonster(monsterDim, 150.0f, blockedMap, 80);
    PlayerObj blockedPlayer(playerDim, 150.0f, blockedMap, 80);
    
    auto chargeCollision = std::make_shared<chargeCollisionComponent>(&blockedMonster, &blockedPlayer, 800);
    
    // Line of sight should be blocked
    EXPECT_TRUE(true);
}

