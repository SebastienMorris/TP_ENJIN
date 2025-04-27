#include "Player.h"

#include <SFML/Window/Event.hpp>

#include "Factory.h"
#include "House.h"
#include "imgui.h"
#include "PowerPlant.h"
#include "../C.hpp"
#include "../Game.hpp"

Player::Player()
{
}

Player::~Player()
{
    delete inventory.at(Materials::MONEY);
    delete inventory.at(Materials::WOOD);
    delete inventory.at(Materials::ELECTRICITY);
}

void Player::ProcessInput(sf::Event ev, RenderWindow& win)
{
    Game* g = Game::me; 
    
    if(ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Right)
    {
        Place(sf::Mouse::getPosition(win).x / C::GRID_SIZE, sf::Mouse::getPosition(win).y / C::GRID_SIZE);
    }

    if(ev.type == Event::MouseButtonReleased && ev.mouseButton.button == Mouse::Right)
    {
        
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
    moraleCheckTimer -= dt;
    if(moraleCheckTimer <= 0)
    {
        CheckMorale();
        moraleCheckTimer = moraleCheckFrequency;
    }
}

void Player::Im()
{
    if(ImGui::CollapsingHeader("Player"))
    {
        ImGui::Value("Population", population);
        ImGui::Value("Morale", morale);
        
        ImGui::DragInt("Money", &inventory.at(Materials::MONEY)->amount, 1, 0, 100);
        ImGui::DragInt("Wood", &inventory.at(Materials::WOOD)->amount, 1, 0, 100);
        ImGui::DragInt("Electricity", &inventory.at(Materials::ELECTRICITY)->amount, 1, 0, 100);
    }
}

void Player::AddResource(Materials resource, int amount)
{
    auto mat = inventory.at(resource);

    if(mat)
        mat->amount += amount;
}

void Player::AddInhabitants(int amount)
{
    population += amount;
    auto elec = inventory.at(Materials::ELECTRICITY);
    if(elec)
        elec->amount -= amount;
}

void Player::Place(int x, int y)
{
    Game* g = Game::me;
    
    if(placeRoad)
    {
        //g->TryPlaceRoad(x, y);
        return;
    }
    
    switch (buildingIndex)
    {
    case 0:
        TryCreateBuilding(x, y, (Building*)BuildingType<House>::Allocate());
        break;

    case 1:
        TryCreateBuilding(x, y, (Building*)BuildingType<Factory>::Allocate());
        break;

    case 2:
        TryCreateBuilding(x, y, (Building*)BuildingType<PowerPlant>::Allocate());
        break;
    }
}

void Player::PlacePreview(int x, int y)
{
    
}

void Player::ConfirmPlacement(int x, int y)
{
}

bool Player::TryCreateBuilding(int x, int y, Building* b)
{
    if(!b) return false;

    Game* g = Game::me;
    
    auto cost = b->GetCost();
    auto mat = inventory.at(cost.type);
    
    if(!mat)
    {
        delete b;
        return false;
    }

    if(mat->amount < cost.amount)
    {
        delete b;
        return false;
    }

    if(!g->TryPlaceBuilding(x, y, b))
    {
        delete b;
        return false;
    }
    
    mat->amount -= cost.amount;
    return true;
}

void Player::CheckMorale()
{
    auto elec = inventory.at(ELECTRICITY);
    if(elec->amount < 0)
    {
        morale -= 0.5f;
    }
    else if(morale < 0)
    {
        morale += 0.5f;
    }
}

