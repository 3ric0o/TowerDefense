#pragma once
#include "raylib.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

enum class AnimationState {
    WALK_DOWN,
    WALK_UP,
    WALK_LEFT,
    WALK_RIGHT,
    DEATH,
    NONE
};

struct Animation {
    Texture2D texture;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int framesSpeed;
};

class Enemy {
public:
    Enemy(float x, float y, float speed, int health, int damage);
    virtual ~Enemy();
    
    virtual void Update(float deltaTime);
    virtual void Draw() const;
    virtual void TakeDamage(int amount);
    virtual bool IsAlive() const;
    virtual bool IsDeathAnimationFinished() const;
    virtual std::string GetType() const = 0;
    
    void SetTargetPosition(float x, float y);
    void SetPosition(float x, float y);
    Vector2 GetPosition() const;
    
    void SetState(AnimationState state);
    
    float GetSpeed() const;
    int GetHealth() const;
    int GetDamage() const;
    void SetSpeedMultiplier(float multiplier);
    void ResetSpeed();

    void SetPath(const std::vector<Vector2>& newPath);
    void ClearPath();
    bool HasPath() const;
    
protected:
    Vector2 position;
    Vector2 targetPosition;
    float speed;
    float maxSpeed;
    int health;
    int maxHealth;
    int damage;
    bool alive;
    bool deathAnimationFinished;
    
    std::unordered_map<AnimationState, Animation> animations;
    AnimationState currentState;
    int currentFrame;
    int framesCounter;
    float scale;
    
    Vector2 direction;

    std::vector<Vector2> path;
    int currentPathIndex;
    const float pathNodeRadius = 10.0f; // How close enemy needs to get to a path node
    
    void UpdateAnimation(float deltaTime);
    void SetAnimationState(AnimationState newState);
    virtual void LoadAnimations() = 0; 
    
    void AddAnimation(
        AnimationState state, 
        const char* texturePath, 
        int frameCount, 
        int framesSpeed = 8
    );
    virtual void FollowPath(float deltaTime);
    virtual void UpdateDirection();
};

std::unique_ptr<Enemy> CreateEnemy(const std::string& type, float x, float y);