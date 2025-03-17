#include "Goblin.h"

Goblin::Goblin(float x, float y)
    : Enemy(x, y, 70.0f, 100, 15)
{
    LoadAnimations();
    SetAnimationState(AnimationState::NONE);
}

void Goblin::LoadAnimations()
{
    AddAnimation(AnimationState::WALK_DOWN, "resources/FIELD ENEMIES/2/D_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/FIELD ENEMIES/2/U_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/FIELD ENEMIES/2/S_Walk.png", 6, 10);
    animations[AnimationState::WALK_LEFT] = animations[AnimationState::WALK_RIGHT];
    AddAnimation(AnimationState::DEATH, "resources/FIELD ENEMIES/2/D_Death.png", 10, 8);
}

std::string Goblin::GetType() const
{
    return "Goblin";
}