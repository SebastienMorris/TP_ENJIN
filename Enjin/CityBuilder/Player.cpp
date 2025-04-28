#include "Player.h"

#include <SFML/Window/Event.hpp>

#include "Factory.h"
#include "House.h"
#include "imgui.h"
#include "PowerPlant.h"
#include "Road.h"
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
        Place(Mouse::getPosition(win).x / C::GRID_SIZE, Mouse::getPosition(win).y / C::GRID_SIZE);
    }

    if(ev.type == Event::MouseButtonReleased && ev.mouseButton.button == Mouse::Right)
    {
        ConfirmPlacement(Mouse::getPosition(win).x / C::GRID_SIZE, Mouse::getPosition(win).y / C::GRID_SIZE);
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

    if(ev.type == Event::MouseMoved)
    {
        UpdatePreviews(Mouse::getPosition(win).x / C::GRID_SIZE, Mouse::getPosition(win).y / C::GRID_SIZE);
    }

    if(ev.type == Event::KeyPressed)
    {
        if(ev.key.code == Keyboard::Space)
        {
            placeRoad = true;
        }
    }

    if(ev.type == Event::KeyReleased)
    {
        if(ev.key.code == Keyboard::Space)
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

void Player::Draw(sf::RenderWindow& win)
{
    if(roadPreview)
        roadPreview->Draw(win);

    if(buildingPreview)
        buildingPreview->Draw(win);
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

void Player::UpdatePreviews(int x, int y)
{
    Game* g = Game::me;
    if(roadPreview)
    {
        if(roadPreview->GetPosition().x != x || roadPreview->GetPosition().y != y)
        {
            roadPreview->SetPosition(x, y);
            
            if(g->CheckRoadPlacement(x, y))
                roadPreview->SetOutlineColour(Color::Green);
            else
                roadPreview->SetOutlineColour(Color::Red);
        }
    }

    if(buildingPreview)
    {
        if(buildingPreview->GetPosition().x != x || buildingPreview->GetPosition().y != y)
        {
            buildingPreview->SetPosition(x, y);
            
            if(g->CheckBuildingPlacement(x, y, buildingPreview->GetSize()))
                buildingPreview->SetOutlineColour(Color::Green);
            else
                buildingPreview->SetOutlineColour(Color::Red);
        }
    }
}

void Player::Place(int x, int y)
{
    Game* g = Game::me;
    
    if(placeRoad)
    {
        PlaceRoadPreview(x, y);
        return;
    }
    
    switch (buildingIndex)
    {
    case 0:
        PlaceBuildingPreview(x, y, (Building*)BuildingType<House>::Allocate());
        break;

    case 1:
        PlaceBuildingPreview(x, y, (Building*)BuildingType<Factory>::Allocate());
        break;

    case 2:
        PlaceBuildingPreview(x, y, (Building*)BuildingType<PowerPlant>::Allocate());
        break;
    }
}

void Player::PlaceRoadPreview(int x, int y)
{
    Game* g = Game::me;
    roadPreview = new Road({x,y});
        
    if(g->CheckRoadPlacement(x,y))
        roadPreview->SetOutlineColour(Color::Green);
    else
        roadPreview->SetOutlineColour(Color::Red);

    roadPreview->SetOutline(true);
}

void Player::PlaceBuildingPreview(int x, int y, Building* b)
{
    Game* g = Game::me;
    buildingPreview = b;
    buildingPreview->SetPosition(x, y);
    
    if(g->CheckBuildingPlacement(x, y, buildingPreview->GetSize()))
        buildingPreview->SetOutlineColour(Color::Green);
    else
        buildingPreview->SetOutlineColour(Color::Red);

    buildingPreview->SetOutline(true);
}

void Player::ConfirmPlacement(int x, int y)
{
    Game* g = Game::me;

    if(roadPreview)
    {
        if(g->CheckRoadPlacement(x, y))
        {
            //Confirm
            roadPreview->Confirm();
            g->PlaceRoad(roadPreview);
            roadPreview = nullptr;
        }
        else
        {
            delete roadPreview;
            roadPreview = nullptr;
        }
    }
    if(buildingPreview)
    {
        if(g->CheckBuildingPlacement(x, y, buildingPreview->GetSize()))
        {
            //Confirm
            buildingPreview->Confirm();
            g->PlaceBuilding(buildingPreview);
            buildingPreview = nullptr;
        }
        else
        {
            delete buildingPreview;
            buildingPreview = nullptr;
        }
    }
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

