﻿#include "Factory.h"

#include <SFML/Graphics/Color.hpp>

Factory::Factory() : Building(5, sf::Color::Black, Material(Materials::MONEY, 7), Material(Materials::ELECTRICITY, 2), Material(Materials::WOOD, 1))
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

void Factory::Confirm()
{
    Building::Confirm();
    
}
