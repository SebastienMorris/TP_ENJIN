#include "PowerPlant.h"

#include <SFML/Graphics/Color.hpp>

PowerPlant::PowerPlant() : Building(5, sf::Color::Yellow, Material(Materials::MONEY, 1), Material(Materials::ELECTRICITY, 1))
{
    Produce();
}

void PowerPlant::Update(double dt)
{
    Building::Update(dt);
}

void PowerPlant::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
