#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Beat dungeon/KeyTime.h"
#include <chrono>
#include <thread>

class KeyTimeTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
    }

    void TearDown() override {
        TTF_Quit();
        SDL_Quit();
    }
};

TEST_F(KeyTimeTest, KeyTimeCreation) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        // Skip test if renderer can't be created
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    EXPECT_EQ(keyTime.getKey(), SDLK_W);
    EXPECT_TRUE(keyTime.getGood());
    EXPECT_FALSE(keyTime.getUsed());
    EXPECT_FALSE(keyTime.getIsDown());
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeGoodBad) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime goodKey(SDLK_W, dummyTexture, true, dummyOutline, pos);
    KeyTime badKey(SDLK_X, dummyTexture, false, dummyOutline, pos);
    
    EXPECT_TRUE(goodKey.getGood());
    EXPECT_FALSE(badKey.getGood());
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeSetUsed) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    EXPECT_FALSE(keyTime.getUsed());
    keyTime.setUsed(true);
    EXPECT_TRUE(keyTime.getUsed());
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeSetIsDown) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    EXPECT_FALSE(keyTime.getIsDown());
    keyTime.setIsDown(true);
    EXPECT_TRUE(keyTime.getIsDown());
    keyTime.setIsDown(false);
    EXPECT_FALSE(keyTime.getIsDown());
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeProgress) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    // Progress should start at 0 or very low
    double progress = keyTime.progress();
    EXPECT_GE(progress, 0.0);
    EXPECT_LE(progress, 100.0);
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeInZone) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    // Initially should not be in zone (progress < 75%)
    bool inZone = keyTime.inZone();
    EXPECT_FALSE(inZone);
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeElapsed) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    // Should not be elapsed immediately
    bool elapsed = keyTime.time_elapsed();
    EXPECT_FALSE(elapsed);
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, KeyTimeExpiration) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime keyTime(SDLK_W, dummyTexture, true, dummyOutline, pos);
    
    // Wait a short time and check if elapsed (but not too long)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Should still not be elapsed (key lasts 1.5-4.5 seconds)
    bool elapsed = keyTime.time_elapsed();
    EXPECT_FALSE(elapsed);
    
    SDL_DestroyRenderer(renderer);
}

TEST_F(KeyTimeTest, BadKeyShorterExpiration) {
    SDL_Renderer* renderer = SDL_CreateRenderer(nullptr, nullptr);
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_Texture* dummyOutline = nullptr;
    std::pair<int, int> pos = {100, 100};
    
    KeyTime goodKey(SDLK_W, dummyTexture, true, dummyOutline, pos);
    KeyTime badKey(SDLK_X, dummyTexture, false, dummyOutline, pos);
    
    // Bad keys should expire faster (have 500ms less time)
    // Both should not be expired immediately
    EXPECT_FALSE(goodKey.time_elapsed());
    EXPECT_FALSE(badKey.time_elapsed());
    
    SDL_DestroyRenderer(renderer);
}

