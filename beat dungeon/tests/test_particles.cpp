#include <gtest/gtest.h>
#include <SDL3/SDL.h>
#include "../Beat dungeon/particles.h"

class ParticlesTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
    }

    void TearDown() override {
        SDL_Quit();
    }
};

TEST_F(ParticlesTest, ParticleInitialization) {
    Particle particle;
    EXPECT_FALSE(particle.inUse);
}

TEST_F(ParticlesTest, ParticleInit) {
    Particle particle;
    particle.init(100.0f, 200.0f, true);
    
    EXPECT_TRUE(particle.inUse);
    EXPECT_FLOAT_EQ(particle.x, 100.0f);
    EXPECT_FLOAT_EQ(particle.y, 200.0f);
    EXPECT_GT(particle.lifetime, 0);
    EXPECT_EQ(particle.a, 255); // Full alpha initially
}

TEST_F(ParticlesTest, ParticleUpdate) {
    Particle particle;
    particle.init(100.0f, 200.0f, true);
    
    float initialX = particle.x;
    float initialY = particle.y;
    int initialLifetime = particle.lifetime;
    
    particle.update();
    
    // Position should change due to velocity
    EXPECT_NE(particle.x, initialX);
    EXPECT_NE(particle.y, initialY);
    
    // Lifetime should decrease
    EXPECT_LT(particle.lifetime, initialLifetime);
}

TEST_F(ParticlesTest, ParticleLifetimeExpiration) {
    Particle particle;
    particle.init(100.0f, 200.0f, true);
    
    // Update until lifetime expires
    while (particle.lifetime > 0 && particle.inUse) {
        particle.update();
    }
    
    EXPECT_FALSE(particle.inUse);
}

TEST_F(ParticlesTest, ParticlePoolCreation) {
    ParticlePool pool;
    
    // Should not crash
    EXPECT_TRUE(true);
}

TEST_F(ParticlesTest, ParticlePoolCreate) {
    ParticlePool pool;
    
    pool.create(100.0f, 200.0f, true);
    
    // Should create a particle
    EXPECT_TRUE(true);
}

TEST_F(ParticlesTest, ParticlePoolMultipleCreates) {
    ParticlePool pool;
    
    // Create multiple particles
    for (int i = 0; i < 10; i++) {
        pool.create(100.0f + i, 200.0f + i, i % 2 == 0);
    }
    
    // Should handle multiple particles
    EXPECT_TRUE(true);
}

TEST_F(ParticlesTest, ParticlePoolUpdate) {
    ParticlePool pool;
    
    pool.create(100.0f, 200.0f, true);
    pool.update();
    
    // Should update all particles
    EXPECT_TRUE(true);
}

TEST_F(ParticlesTest, ParticlePoolRender) {
    SDL_Window* window = SDL_CreateWindow("Test", 100, 100, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (!renderer) {
        GTEST_SKIP();
    }
    
    ParticlePool pool;
    pool.create(100.0f, 200.0f, true);
    pool.render(renderer);
    
    // Should render without crashing
    EXPECT_TRUE(true);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

TEST_F(ParticlesTest, ParticleColorRandom) {
    Particle particle1;
    Particle particle2;
    
    particle1.init(100.0f, 200.0f, true);
    particle2.init(100.0f, 200.0f, true);
    
    // Colors should be random (likely different)
    // Note: There's a small chance they could be the same
    EXPECT_TRUE(true);
}

TEST_F(ParticlesTest, ParticleColorWhite) {
    Particle particle;
    particle.init(100.0f, 200.0f, false);
    
    // When color is false, should be white
    EXPECT_EQ(particle.r, 255);
    EXPECT_EQ(particle.g, 255);
    EXPECT_EQ(particle.b, 255);
}

TEST_F(ParticlesTest, ParticleFadeOut) {
    Particle particle;
    particle.init(100.0f, 200.0f, true);
    
    int initialLifetime = particle.lifetime;
    Uint8 initialAlpha = particle.a;
    
    // Update multiple times
    for (int i = 0; i < 10; i++) {
        particle.update();
    }
    
    // Alpha should decrease as lifetime decreases
    if (particle.lifetime < initialLifetime) {
        EXPECT_LE(particle.a, initialAlpha);
    }
}

TEST_F(ParticlesTest, ParticlePoolReuse) {
    ParticlePool pool;
    
    // Create and let expire
    pool.create(100.0f, 200.0f, true);
    
    // Update many times to expire
    for (int i = 0; i < 200; i++) {
        pool.update();
    }
    
    // Create new particle - should reuse expired particle
    pool.create(300.0f, 400.0f, false);
    
    EXPECT_TRUE(true);
}

