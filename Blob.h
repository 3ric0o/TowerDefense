#pragma once
#include "Enemy.h"

class Blob : public Enemy
{
public:
    Blob(float x, float y);
    
    std::string GetType() const override;
    
private:
    void LoadAnimations() override;
};