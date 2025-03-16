#include "Goblin.h"

Goblin::Goblin(float x, float y)
    : Enemy(x, y, 70.0f, 100, 15)
{
    LoadAnimations();
    SetAnimationState(AnimationState::IDLE);
}

void Goblin::LoadAnimations()
{
    
    AddAnimation(AnimationState::WALK_DOWN, "resources/enemies/goblin_walk_down.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/enemies/goblin_walk_up.png", 6, 10);
    AddAnimation(AnimationState::WALK_LEFT, "resources/enemies/goblin_walk_left.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/enemies/goblin_walk_right.png", 6, 10);
    AddAnimation(AnimationState::ATTACK, "resources/enemies/goblin_attack.png", 8, 12);
    AddAnimation(AnimationState::DEATH, "resources/enemies/goblin_death.png", 10, 8);
}

std::string Goblin::GetType() const
{
    return "Goblin";
}