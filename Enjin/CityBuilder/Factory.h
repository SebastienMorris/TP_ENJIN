#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include "Building.h"

class Factory : Building
{
public:
    Factory();

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
    void Confirm() override;
};
