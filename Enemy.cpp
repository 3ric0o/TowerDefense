#include "Enemy.h"
#include "Blob.h"
#include "Goblin.h"
#include "Rat.h"
#include <cmath>
#include <iostream>
#include <ranges>

Enemy::Enemy(float x, float y, float speed, int health, int damage)
    : position{x, y}, targetPosition{x, y}, speed(speed), health(health), 
      maxHealth(health), damage(damage), alive(true),
      deathAnimationFinished(false), currentFrame(0), framesCounter(0), scale(2.0f),direction{0, 1}
{}

Enemy::~Enemy()
{
    for (auto& anim : animations | std::views::values)
    {
        UnloadTexture(anim.texture);
    }
}

void Enemy::Update(float deltaTime)
{
    if (!alive)
    {
        if (currentState != AnimationState::DEATH)
        {
            SetAnimationState(AnimationState::DEATH);
        }
        UpdateAnimation(deltaTime);
        return;
    }
    UpdateDirection();
    MoveTowardsTarget(deltaTime);
    UpdateAnimation(deltaTime);
}

void Enemy::Draw() const
{
    auto it = animations.find(currentState);
    if (it != animations.end())
        {
        const Animation& anim = it->second;
        
        bool shouldFlip = (currentState == AnimationState::WALK_LEFT);
        
        Rectangle source =
            { static_cast<float>(currentFrame * anim.frameWidth),
            0,
            static_cast<float>(shouldFlip ? -anim.frameWidth : anim.frameWidth),
            static_cast<float>(anim.frameHeight)
        };
        
        Rectangle dest =
            {
            position.x,
            position.y,
            anim.frameWidth * scale,
            anim.frameHeight * scale
        };
        
        Vector2 origin =
            { 
            anim.frameWidth * scale / 2.0f,
            anim.frameHeight * scale / 2.0f
        };
        
        DrawTexturePro(anim.texture, source, dest, origin, 0.0f, WHITE);
    }
    else
    {
        // Fallback if animation not found
        DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), 20, RED);
    }
    
    // Draw health bar above enemy
    const int healthBarWidth = 40;
    const int healthBarHeight = 5;
    const int healthBarX = static_cast<int>(position.x - healthBarWidth / 2);
    const int healthBarY = static_cast<int>(position.y - 30);
    
    // Health bar background
    DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, RED);
    
    // Current health
    int currentHealthWidth = static_cast<int>((static_cast<float>(health) / maxHealth) * healthBarWidth);
    DrawRectangle(healthBarX, healthBarY, currentHealthWidth, healthBarHeight, GREEN);
}

void Enemy::TakeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        health = 0;
        alive = false;
        SetAnimationState(AnimationState::DEATH);
    }
}

bool Enemy::IsAlive() const { return alive; }

bool Enemy::IsDeathAnimationFinished() const { return !alive && deathAnimationFinished; }

void Enemy::SetTargetPosition(float x, float y)
{
    targetPosition.x = x;
    targetPosition.y = y;
}

void Enemy::SetPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

Vector2 Enemy::GetPosition() const
{
    return position;
}

void Enemy::SetState(AnimationState state)
{
    SetAnimationState(state);
}

float Enemy::GetSpeed() const
{
    return speed;
}

int Enemy::GetHealth() const
{
    return health;
}

int Enemy::GetDamage() const
{
    return damage;
}

void Enemy::UpdateAnimation(float deltaTime)
{
    auto it = animations.find(currentState);
    if (it != animations.end())
    {
        const Animation& anim = it->second;
        framesCounter++;
        
        if (framesCounter >= (60 / anim.framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;
            
            if (currentFrame >= anim.frameCount)
            {
                if (currentState == AnimationState::DEATH)
                {
                    currentFrame = anim.frameCount - 1;
                    deathAnimationFinished = true; // Mark death animation as finished
                }
                else
                {
                    currentFrame = 0;
                }
            }
        }
    }
}

void Enemy::SetAnimationState(AnimationState newState)
{
    if (currentState != newState)
    {
        auto it = animations.find(newState);
        if (it != animations.end())
        {
            currentState = newState;
            currentFrame = 0;
            framesCounter = 0;
        }
    }
}

void Enemy::AddAnimation(
    AnimationState state, 
    const char* texturePath, 
    int frameCount, 
    int framesSpeed)
{
    Texture2D texture = LoadTexture(texturePath);
    
    if (texture.id > 0)
    {
        Animation anim;
        anim.texture = texture;
        anim.frameCount = frameCount;
        anim.frameWidth = texture.width / frameCount;
        anim.frameHeight = texture.height;
        anim.framesSpeed = framesSpeed;
        
        animations[state] = anim;
    }
    else
    {
        std::cout << "Failed to load texture: " << texturePath << '\n';
    }
}

void Enemy::MoveTowardsTarget(float deltaTime)
{
    // direction vector
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    
    // magnitude of direction vector
    float distance = sqrt(dx * dx + dy * dy);
    
    if (distance < 1.0f) { return; }
    
    // Normalize vector
    dx /= distance;
    dy /= distance;
    
    // Update direction for animation state
    direction.x = dx;
    direction.y = dy;
    
    // Move towards target
    position.x += dx * speed * deltaTime;
    position.y += dy * speed * deltaTime;
}

void Enemy::UpdateDirection()
{
    // Calculate normalized direction to target
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > 1.0f) {
        // Normalize
        dx /= distance;
        dy /= distance;
        
        direction.x = dx;
        direction.y = dy;
        
        if (fabs(dx) > fabs(dy))
        {
            if (dx > 0)
            {
                SetAnimationState(AnimationState::WALK_RIGHT);
            }
            else
            {
                SetAnimationState(AnimationState::WALK_LEFT);
            }
        }
        else
        {
            if (dy > 0)
            {
                SetAnimationState(AnimationState::WALK_DOWN);
            }
            else
            {
                SetAnimationState(AnimationState::WALK_UP);
            }
        }
    }
    else
    {
        SetAnimationState(AnimationState::NONE);
    }
}

std::unique_ptr<Enemy> CreateEnemy(const std::string& type, float x, float y)
{
    if (type == "Blob")
    {
        return std::make_unique<Blob>(x, y);
    }
    if (type == "Goblin")
    {
        return std::make_unique<Goblin>(x, y);
    }
    if (type == "Rat")
    {
        return std::make_unique<Rat>(x, y);
    }
    return nullptr;
}