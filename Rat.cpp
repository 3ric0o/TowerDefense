#include "Rat.h"

Rat::Rat(float x, float y)
    : Enemy(x, y, 90.0f, 50, 5)
{
    LoadAnimations();
    SetAnimationState(AnimationState::IDLE);
}

void Rat::LoadAnimations() 
{
    AddAnimation(AnimationState::WALK_DOWN, "resources/enemies/rat_walk_down.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/enemies/rat_walk_up.png", 6, 10);
    AddAnimation(AnimationState::WALK_LEFT, "resources/enemies/rat_walk_left.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/enemies/rat_walk_right.png", 6, 10);
    AddAnimation(AnimationState::ATTACK, "resources/enemies/rat_attack.png", 8, 12);
    AddAnimation(AnimationState::DEATH, "resources/enemies/rat_death.png", 10, 8);
}

std::string Rat::GetType() const
{
    return "Rat";
}