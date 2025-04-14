#include "Factory.h"

#include <SFML/Graphics/Color.hpp>

Factory::Factory(sf::Vector2i spawnPos) : Building(spawnPos, 5, sf::Color::Red, Material(Materials::MONEY, 1), Material(Materials::WOOD, 1))
{
}

void Factory::Update(double dt)
{
    Building::Update(dt);
}

void Factory::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
