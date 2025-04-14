#include "House.h"

#include <SFML/Graphics/Color.hpp>

House::House(sf::Vector2i spawnPos) : Building(spawnPos, 3, sf::Color::Blue, Material(Materials::WOOD, 1), Material(MONEY, 1))
{
    
}

void House::Update(double dt)
{
    Building::Update(dt);
}

void House::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
