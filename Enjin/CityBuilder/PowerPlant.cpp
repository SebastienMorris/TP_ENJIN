#include "PowerPlant.h"

#include <SFML/Graphics/Color.hpp>

PowerPlant::PowerPlant() : Building(5, sf::Color::Yellow, Material(Materials::MONEY, 10), Material(Materials::ELECTRICITY, 0), Material(Materials::ELECTRICITY, 3))
{
    SetProductionState(true, 5.0f);
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

