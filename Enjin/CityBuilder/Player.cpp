#include "Player.h"

#include <SFML/Window/Event.hpp>

#include "Factory.h"
#include "House.h"
#include "PowerPlant.h"
#include "../C.hpp"
#include "../Game.hpp"

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
    
    if(ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Right)
    {
        Place(sf::Mouse::getPosition(win).x / C::GRID_SIZE, sf::Mouse::getPosition(win).y / C::GRID_SIZE);
    }

    if(ev.type == Event::MouseWheelScrolled)
    {
        if(ev.mouseWheelScroll.delta > 0)
        {
            buildingIndex++;
            if(buildingIndex >= nbBuildingTypes)
                buildingIndex = 0;
        }
        else if(ev.mouseWheelScroll.delta < 0)
        {
            buildingIndex--;
            if(buildingIndex < 0)
                buildingIndex = nbBuildingTypes - 1;
        }   
    }

    if(ev.type == Event::KeyPressed)
    {
        if(ev.key.code == sf::Keyboard::Space)
        {
            placeRoad = true;
        }
    }

    if(ev.type == Event::KeyReleased)
    {
        if(ev.key.code == sf::Keyboard::Space)
        {
            placeRoad = false;
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

void Player::Place(int x, int y)
{
    Game* g = Game::me;
    
    if(placeRoad)
    {
        g->TryPlaceRoad(x, y);
        return;
    }
    
    switch (buildingIndex)
    {
    case 0:
        g->TryPlaceBuilding(x, y, (Building*)BuildingType<House>::Allocate());
        break;

    case 1:
        g->TryPlaceBuilding(x, y, (Building*)BuildingType<Factory>::Allocate());
        break;

    case 2:
        g->TryPlaceBuilding(x, y, (Building*)BuildingType<PowerPlant>::Allocate());
        break;
    }
}

