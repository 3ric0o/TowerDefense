#include "Blob.h"

Blob::Blob(float x, float y)
    : Enemy(x, y, 50.0f, 150, 10)
{
    LoadAnimations();
    SetAnimationState(AnimationState::IDLE);
}

void Blob::LoadAnimations()
{
    AddAnimation(AnimationState::WALK_DOWN, "resources/FIELD ENEMIES/1/D_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/FIELD ENEMIES/1/D_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_LEFT, "resources/FIELD ENEMIES/1/S_Walk.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/FIELD ENEMIES/1/S_Walk.png", 6, 10);
    AddAnimation(AnimationState::DEATH, "resources/FIELD ENEMIES/1/D_Death.png", 6, 10);
}

std::string Blob::GetType() const
{
    return "Blob";
}