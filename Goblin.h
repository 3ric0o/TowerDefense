#pragma once
#include "Enemy.h"

class Goblin : public Enemy
{
public:
    Goblin(float x, float y);
    
    std::string GetType() const override;
    
protected:
    void LoadAnimations() override;
};