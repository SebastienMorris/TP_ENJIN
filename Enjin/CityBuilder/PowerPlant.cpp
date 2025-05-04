#include "PowerPlant.h"

#include <SFML/Graphics/Color.hpp>

PowerPlant::PowerPlant() : Building(5, sf::Color::Yellow, Material(Materials::MONEY, 1), Material(Materials::ELECTRICITY, 5))
{
}

void PowerPlant::Update(double dt)
{
    Building::Update(dt);
}

void PowerPlant::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}

void PowerPlant::Confirm()
{
    Building::Confirm();
    Produce();
}

