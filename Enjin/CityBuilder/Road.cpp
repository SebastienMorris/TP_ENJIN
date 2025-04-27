#include "Road.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../C.hpp"

Road::Road(sf::Vector2i spawnPos) : pos(spawnPos)
{
    sprite = new sf::RectangleShape({static_cast<float>(C::GRID_SIZE), static_cast<float>(C::GRID_SIZE)});
    sprite->setFillColor(sf::Color::White);
    sprite->setOrigin(C::GRID_SIZE * 0.5f, C::GRID_SIZE * 0.5f);
    
    SyncPos();
}

void Road::Update(double dt)
{
    SyncPos();
}

void Road::Draw(sf::RenderWindow& win)
{
    win.draw(*sprite);
}

void Road::Confirm()
{
    confirmed = true;
    SetOutline(false);
}

void Road::SyncPos()
{
    sf::Vector2f p = {static_cast<float>(pos.x * C::GRID_SIZE), static_cast<float>(pos.y * C::GRID_SIZE)};
    sprite->setPosition(p);
}
