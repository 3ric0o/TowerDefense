#include "Enemy.h"
#include "Blob.h"
#include "Goblin.h"
#include "Rat.h"
#include <cmath>
#include <iostream>
#include <ranges>

Enemy::Enemy(float x, float y, float speed, int health, int damage)
    : position{x, y}, targetPosition{x, y}, speed(speed), maxSpeed(speed), health(health), 
      maxHealth(health), damage(damage), alive(true),
      deathAnimationFinished(false), currentFrame(0), framesCounter(0), scale(2.0f), direction{0, 1}
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
    FollowPath(deltaTime);
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

bool Enemy::IsDeathAnimationFinished() const
{
    if (!alive)
    {
        // If the current state is DEATH and we've reached the last frame
        if (currentState == AnimationState::DEATH)
        {
            Animation deathAnimation = animations.at(AnimationState::DEATH);
            return currentFrame >= deathAnimation.frameCount - 1;
        }
    }
    return false;
}

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

void Enemy::SetSpeedMultiplier(float multiplier)
{
    speed = maxSpeed * multiplier;
}

void Enemy::ResetSpeed()
{
    speed = maxSpeed;
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

void Enemy::UpdateDirection()
{
    if (fabs(direction.x) < 0.01f && fabs(direction.y) < 0.01f)
        {
        SetAnimationState(AnimationState::NONE);
        return;
    }

    // Determine animation based on dominant direction component
    if (fabs(direction.x) > fabs(direction.y))
    {
        // Horizontal movement is dominant
        if (direction.x > 0)
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
        // Vertical movement is dominant
        if (direction.y > 0)
        {
            SetAnimationState(AnimationState::WALK_DOWN);
        }
        else
        {
            SetAnimationState(AnimationState::WALK_UP);
        }
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

void Enemy::SetPath(const std::vector<Vector2>& newPath)
{
    path = newPath;
    currentPathIndex = 0;
    
    // If we have a path, set the first node as our target
    if (!path.empty())
    {
        targetPosition.x = path[0].x * 32 * 2.0f + (32 * 2.0f / 2); // Center of tile
        targetPosition.y = path[0].y * 32 * 2.0f + (32 * 2.0f / 2); // Center of tile
    }
}

void Enemy::ClearPath()
{
    path.clear();
    currentPathIndex = 0;
}

bool Enemy::HasPath() const { return !path.empty(); }

void Enemy::FollowPath(float deltaTime)
{
    if (currentPathIndex >= path.size()) { return; }

    // path node to world coordinates 
    float nodeX = path[currentPathIndex].x * 32 * 2.0f + (32 * 2.0f / 2);
    float nodeY = path[currentPathIndex].y * 32 * 2.0f + (32 * 2.0f / 2);

    // Direction to current path node
    Vector2 moveDir =
        {
        nodeX - position.x,
        nodeY - position.y
    };

    // Distance to current path node
    float distance = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);

    // If close enough to the current node, target the next one
    if (distance < pathNodeRadius)
    {
        currentPathIndex++;

        if (currentPathIndex < path.size())
        {
            // Update target to the next node
            nodeX = path[currentPathIndex].x * 32 * 2.0f + (32 * 2.0f / 2);
            nodeY = path[currentPathIndex].y * 32 * 2.0f + (32 * 2.0f / 2);

            // Update direction
            moveDir.x = nodeX - position.x;
            moveDir.y = nodeY - position.y;
            distance = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        }
        else
        {
            return;
        }
    }

    // Normalize direction
    if (distance > 0)
    {
        moveDir.x /= distance;
        moveDir.y /= distance;
        
        direction = moveDir;
        UpdateDirection();

        // Move towards target
        position.x += moveDir.x * speed * deltaTime;
        position.y += moveDir.y * speed * deltaTime;
    }
}
