#pragma once
#include "Enemy.h"

class Rat : public Enemy
{
public:
    Rat(float x, float y);
    
    void Update(float deltaTime) override;
    void MoveTowardsTarget(float deltaTime) override;
    std::string GetType() const override;
    
protected:
    void LoadAnimations() override;
};