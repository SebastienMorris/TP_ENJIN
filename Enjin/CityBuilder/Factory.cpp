#include "Factory.h"

#include <SFML/Graphics/Color.hpp>

Factory::Factory() : Building(5, sf::Color::Red, Material(Materials::MONEY, 1), Material(Materials::WOOD, 1))
{
    SetProductionState(true, 5.0);
}

void Factory::Update(double dt)
{
    Building::Update(dt);
}

void Factory::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
