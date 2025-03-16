#include "Blob.h"

Blob::Blob(float x, float y)
    : Enemy(x, y, 50.0f, 150, 10)
{
    LoadAnimations();
    SetAnimationState(AnimationState::IDLE);
}

void Blob::LoadAnimations()
{
    AddAnimation(AnimationState::WALK_DOWN, "resources/enemies/blob_walk_down.png", 6, 10);
    AddAnimation(AnimationState::WALK_UP, "resources/enemies/blob_walk_up.png", 6, 10);
    AddAnimation(AnimationState::WALK_LEFT, "resources/enemies/blob_walk_left.png", 6, 10);
    AddAnimation(AnimationState::WALK_RIGHT, "resources/enemies/blob_walk_right.png", 6, 10);
    AddAnimation(AnimationState::ATTACK, "resources/enemies/blob_attack.png", 8, 12);
    AddAnimation(AnimationState::DEATH, "resources/enemies/blob_death.png", 10, 8);
}

std::string Blob::GetType() const
{
    return "Blob";
}