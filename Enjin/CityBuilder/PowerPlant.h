#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include "Building.h"

class PowerPlant : Building
{
public:
    PowerPlant();

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
};
