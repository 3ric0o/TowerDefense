#pragma once
#include "raylib.h"
#include <string>
#include <memory>
#include <unordered_map>

enum class AnimationState {
    IDLE,
    WALK_DOWN,
    WALK_UP,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK,
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
    virtual std::string GetType() const = 0;
    
    void SetTargetPosition(float x, float y);
    void SetPosition(float x, float y);
    Vector2 GetPosition() const;
    
    void SetState(AnimationState state);
    
    float GetSpeed() const;
    int GetHealth() const;
    int GetDamage() const;
    
protected:
    Vector2 position;
    Vector2 targetPosition;
    float speed;
    int health;
    int maxHealth;
    int damage;
    bool alive;
    
    std::unordered_map<AnimationState, Animation> animations;
    AnimationState currentState;
    int currentFrame;
    int framesCounter;
    float scale;
    
    Vector2 direction;
    
    void UpdateAnimation(float deltaTime);
    void SetAnimationState(AnimationState newState);
    virtual void LoadAnimations() = 0; 
    
    void AddAnimation(
        AnimationState state, 
        const char* texturePath, 
        int frameCount, 
        int framesSpeed = 8
    );
    
    virtual void MoveTowardsTarget(float deltaTime);
    virtual void UpdateDirection();
};

std::unique_ptr<Enemy> CreateEnemy(const std::string& type, float x, float y);