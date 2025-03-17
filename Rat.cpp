#include "Rat.h"

Rat::Rat(float x, float y)
    : Enemy(x, y, 90.0f, 50, 5)
{
    LoadAnimations();
    SetAnimationState(AnimationState::NONE);
}

void Rat::LoadAnimations() 
{
    AddAnimation(AnimationState::WALK_DOWN, "resources/FIELD ENEMIES/3/D_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/FIELD ENEMIES/3/U_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/FIELD ENEMIES/3/S_Walk.png", 6, 10);
    animations[AnimationState::WALK_LEFT] = animations[AnimationState::WALK_RIGHT];
    AddAnimation(AnimationState::DEATH, "resources/FIELD ENEMIES/3/U_Death.png", 10, 8);
}

std::string Rat::GetType() const
{
    return "Rat";
}