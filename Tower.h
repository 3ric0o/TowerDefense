#pragma once
#include "raylib.h"
#include "WalkabilityMap.h"
#include <vector>
#include <memory>
#include "Enemy.h"

class Projectile {
public:
    Projectile(Vector2 start, Vector2 target, float speed = 500.0f, int damage = 20);
    
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const;
    Vector2 GetPosition() const;
    int GetDamage() const;
    
private:
    Vector2 position;
    Vector2 direction;
    float speed;
    int damage;
    bool active;
    float radius;
};

class Tower
{
public:
    Tower(float x, float y, float radius = 250.0f);
    ~Tower();
    
    void Update(float deltaTime);
    void Draw() const;
    static bool CanPlaceAt(int tileX, int tileY, const WalkabilityMap& walkMap);
    static void DrawPlacementPreview(int tileX, int tileY, const WalkabilityMap& walkMap);
    void CheckEnemiesInRange(const std::vector<std::unique_ptr<Enemy>>& enemies);
    
    // Getters
    Vector2 GetPosition() const;
    float GetRadius() const;
    bool IsActive() const;
    const std::vector<Projectile>& GetProjectiles() const;

    static constexpr int TOWER_WIDTH = 2;
    static constexpr int TOWER_HEIGHT = 2;
    
private:
    Vector2 position;
    float radius;
    bool active;
    Texture2D texture;
    float scale = 2.0f;
    
    // Animation properties
    int frameCount;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    int frameWidth;
    
    // Projectile properties
    std::vector<Projectile> projectiles;
    float attackCooldown;
    float attackTimer;
    float attackRate; 
};