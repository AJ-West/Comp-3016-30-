#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../Beat dungeon/button.h"

class ButtonTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
        //IMG_Init(IMG_INIT_PNG);
    }

    void TearDown() override {
        //IMG_Quit();
        SDL_Quit();
    }
};

TEST_F(ButtonTest, ButtonCreation) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    EXPECT_TRUE(button.getIsDisplayed());
    EXPECT_FALSE(button.getIsHover());
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonSetIsDisplayed) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    button.setIsDisplayed(false);
    EXPECT_FALSE(button.getIsDisplayed());
    
    button.setIsDisplayed(true);
    EXPECT_TRUE(button.getIsDisplayed());
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonIsWithinRect) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    // Point inside button
    EXPECT_TRUE(button.isWithinRect(150, 125));
    
    // Point outside button (left)
    EXPECT_FALSE(button.isWithinRect(50, 125));
    
    // Point outside button (right)
    EXPECT_FALSE(button.isWithinRect(350, 125));
    
    // Point outside button (above)
    EXPECT_FALSE(button.isWithinRect(150, 50));
    
    // Point outside button (below)
    EXPECT_FALSE(button.isWithinRect(150, 200));
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonCheckHover) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    // Mouse over button
    button.checkHover(150, 125);
    EXPECT_TRUE(button.getIsHover());
    
    // Mouse away from button
    button.checkHover(50, 50);
    EXPECT_FALSE(button.getIsHover());
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonSetFunc) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    bool funcCalled = false;
    auto testFunc = [&funcCalled]() { funcCalled = true; };
    
    button.setFunc(testFunc);
    button.onClick();
    
    EXPECT_TRUE(funcCalled);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonSetLevel) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    int levelLoaded = 0;
    auto levelFunc = [&levelLoaded](int level) { levelLoaded = level; };
    
    button.setLevel(levelFunc, 5);
    button.onClick();
    
    EXPECT_EQ(levelLoaded, 5);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonOnClickWithoutFunc) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    // Should not crash when clicking without function set
    button.onClick();
    
    EXPECT_TRUE(true);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ButtonTest, ButtonBoundaryConditions) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    SDL_Texture* dummyTexture = nullptr;
    SDL_FRect rect = {100.0f, 100.0f, 200.0f, 50.0f};
    
    Button button(true, renderer, dummyTexture, rect);
    
    // Test exact boundaries
    EXPECT_TRUE(button.isWithinRect(100, 100)); // Top-left corner
    EXPECT_TRUE(button.isWithinRect(299, 149)); // Bottom-right corner (exclusive)
    EXPECT_FALSE(button.isWithinRect(300, 150)); // Just outside
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

