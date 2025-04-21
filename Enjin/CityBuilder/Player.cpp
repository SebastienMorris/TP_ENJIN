#include "Player.h"

#include <SFML/Window/Event.hpp>

#include "House.h"
#include "../C.hpp"
#include "../Game.hpp"

class Game;

Player::Player()
{
}

Player::~Player()
{
    delete money;
    delete wood;
}

void Player::ProcessInput(sf::Event ev, RenderWindow& win)
{
    Game* g = Game::me; 
    
    if(ev.mouseButton.button == sf::Mouse::Right)
    {
        g->TryPlaceBuilding(ev.mouseButton.x / C::GRID_SIZE, ev.mouseButton.y / C::GRID_SIZE, (Building*)BuildingType<House>::Allocate());
    }

    if(ev.type == Event::KeyPressed)
    {
        if(ev.key.code == sf::Keyboard::Space)
        {
            g->TryPlaceRoad(sf::Mouse::getPosition(win).x / C::GRID_SIZE, sf::Mouse::getPosition(win).y / C::GRID_SIZE);
        }
    }
}

void Player::Update(double dt)
{
}

void Player::AddMoney(int amount)
{
    money->amount += amount;
}

void Player::AddWood(int amount)
{
    wood->amount += amount;
}

void Player::AddElectricity(int amount)
{
    electricity->amount += amount;
}
