#pragma once
#include "Enemy.h"

class Rat : public Enemy
{
public:
    Rat(float x, float y);
    
    std::string GetType() const override;
    
protected:
    void LoadAnimations() override;
};