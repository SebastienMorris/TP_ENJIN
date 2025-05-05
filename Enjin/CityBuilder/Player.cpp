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
        ConfirmPlacement();
    }

    if(ev.type == Event::MouseWheelScrolled)
    {
        if(ev.mouseWheelScroll.delta > 0)
        {
            buildingIndex++;
            if(buildingIndex >= nbBuildingTypes)
                buildingIndex = 0;

            if(buildingPreview || !roadPreviews.empty())
            {
                delete buildingPreview;
                buildingPreview = nullptr;

                for(auto r : roadPreviews)
                    delete r;

                roadPreviews.clear();
                
                Place(Mouse::getPosition(win).x / C::GRID_SIZE, Mouse::getPosition(win).y / C::GRID_SIZE);
            }
                
        }
        else if(ev.mouseWheelScroll.delta < 0)
        {
            buildingIndex--;
            if(buildingIndex < 0)
                buildingIndex = nbBuildingTypes - 1;

            if(buildingPreview || !roadPreviews.empty())
            {
                delete buildingPreview;
                buildingPreview = nullptr;

                for(auto r : roadPreviews)
                    delete r;

                roadPreviews.clear();
                
                Place(Mouse::getPosition(win).x / C::GRID_SIZE, Mouse::getPosition(win).y / C::GRID_SIZE);
            }
        }   
    }

    if(ev.type == Event::MouseMoved)
    {
        UpdatePreviews(Mouse::getPosition(win).x, Mouse::getPosition(win).y);
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
    for(auto r : roadPreviews)
        r->Draw(win);

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

void Player::UpdatePreviews(int mouseX, int mouseY)
{
    Game* g = Game::me;
    int x = mouseX / C::GRID_SIZE;
    int y = mouseY / C::GRID_SIZE;

    float rx = (float)mouseX / C::GRID_SIZE - x;
    float ry = (float)mouseY / C::GRID_SIZE - y;
    
    if(!roadPreviews.empty())
    {
        if(roadPreviews.size() == 1)
        {
            if(roadPreviews.back()->GetPosition().x != x && roadPreviews.back()->GetPosition().y != y)
            {
                if(rx >= ry)
                {
                    roadPreviewHorizontal = true;
                }
                else
                {
                    roadPreviewHorizontal = false;
                }
            }
            else if(roadPreviews.back()->GetPosition().x != x)
            {
                roadPreviewHorizontal = true;
            }
            else if(roadPreviews.back()->GetPosition().y != y)
            {
                roadPreviewHorizontal = false;
            }
        }
        
        /*for(auto r : roadPreviews)
        {
            if(r->GetPosition().x == x && r->GetPosition().y == y)
                return;
        }*/
        if(roadPreviewHorizontal)
        {
            printf(std::to_string(roadPreviews.back()->GetPosition().x).c_str());
            printf(" / ");
            printf(std::to_string(x).c_str());
            printf(" / ");
            if(roadPreviews.back()->GetPosition().x != x)
            {
                printf("yes");
                int dir = x - roadPreviews[0]->GetPosition().x;

                if(roadPreviews.back()->GetPosition().x < x)
                {
                    if(dir > 0)
                    {
                        AddPreviewRoad(x, roadPreviews.back()->GetPosition().y);
                    }
                    else
                    {
                        RemovePreviewRoad(x, roadPreviews.back()->GetPosition().y);
                    }
                }
                else
                {
                    if(dir < 0)
                    {
                        AddPreviewRoad(x, roadPreviews.back()->GetPosition().y);
                    }
                    else
                    {
                        RemovePreviewRoad(x, roadPreviews.back()->GetPosition().y);
                    }
                }
            }
            
            printf("\n");
        }
        else
        {
            if(roadPreviews.back()->GetPosition().y != y)
            {
                int dir = y - roadPreviews[0]->GetPosition().y;

                if(roadPreviews.back()->GetPosition().y < y)
                {
                    if(dir > 0)
                    {
                        AddPreviewRoad(roadPreviews.back()->GetPosition().x, y);
                    }
                    else
                    {
                        RemovePreviewRoad(roadPreviews.back()->GetPosition().x, y);
                    }
                }
                else
                {
                    if(dir < 0)
                    {
                        AddPreviewRoad(roadPreviews.back()->GetPosition().x, y);
                    }
                    else
                    {
                        RemovePreviewRoad(roadPreviews.back()->GetPosition().x, y);
                    }
                }
            }
        }
        
    }

    if(buildingPreview)
    {
        if(buildingPreview->GetPosition().x != x || buildingPreview->GetPosition().y != y)
        { 
            Vector2i placePos = {x, y};

            if(!g->CheckBuildingPlacement(placePos.x, placePos.y, buildingPreview->GetSize()))
            {
                placePos = TrySnapBuilding(mouseX, mouseY, buildingPreview);
            }

            if(placePos.x < 0 && placePos.y < 0) return;
            
            if(g->CheckBuildingPlacement(placePos.x, placePos.y, buildingPreview->GetSize()))
            {
                buildingPreview->SetOutlineColour(Color::Green);
            }
            else
            {
                buildingPreview->SetOutlineColour(Color::Red);
            }
            
            buildingPreview->SetPosition(placePos.x, placePos.y);
        }
    }
}

Vector2i Player::TrySnapBuilding(int mouseX, int mouseY, Building* building)
{
    Game* g = Game::me;
    int x = mouseX / C::GRID_SIZE;
    int y = mouseY / C::GRID_SIZE;

    float rx = (float)mouseX / C::GRID_SIZE - x;
    float ry = (float)mouseY / C::GRID_SIZE - y;
    
    bool right = g->CheckBuildingPlacement(x + 1, y, building->GetSize());
    bool left = g->CheckBuildingPlacement(x - 1, y, building->GetSize());
    bool up = g->CheckBuildingPlacement(x, y - 1, building->GetSize());
    bool down = g->CheckBuildingPlacement(x, y + 1, building->GetSize());

    if(!right && !left && !up && !down)
        return {-1, -1}; 

    buildingSnapSlots[0].z = rx * (right ? 1.0f : -1.0f);
    buildingSnapSlots[1].z = (1 - rx) * (left ? 1.0f : -1.0f);
    buildingSnapSlots[2].z = (1 - ry) * (up ? 1.0f : -1.0f);
    buildingSnapSlots[3].z = ry * (down ? 1.0f : -1.0f);

    Vector3f snapSlot = buildingSnapSlots[0];
    for(auto slot : buildingSnapSlots)
    {
        if(slot.z >= 0 && slot.z > snapSlot.z)
        {
            snapSlot = slot;
        }
    }

    printf("\n");

    if(snapSlot.z >= 0)
    {
        x += (int)snapSlot.x;
        y += (int)snapSlot.y;
    }

    return {x, y};
}

void Player::Place(int x, int y)
{
    Game* g = Game::me;
    
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

    case 3:
        PlaceRoadPreview(x, y);
        break;
    }
}

void Player::PlaceRoadPreview(int x, int y)
{
    Game* g = Game::me;
    auto r = new Road({x, y});
    roadPreviews.push_back(r);
        
    if(g->CheckRoadPlacement(x,y))
        r->SetOutlineColour(Color::Green);
    else
        r->SetOutlineColour(Color::Red);

    r->SetOutline(true);
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

void Player::ConfirmPlacement()
{
    Game* g = Game::me;

    if(!roadPreviews.empty())
    {
        bool confirm = true;
        for(auto r : roadPreviews)
        {
            if(!g->CheckRoadPlacement(r->GetPosition().x, r->GetPosition().y))
            {
                confirm = false;
                break;
            }
        }
            
        if(confirm)
        {
            //Confirm
            for(auto r : roadPreviews)
            {
                r->Confirm();
                g->PlaceRoad(r);
            }
            roadPreviews.clear();
        }
        else
        {
            for(auto r : roadPreviews)
                delete r;

            roadPreviews.clear();
        }
    }
    if(buildingPreview)
    {
        if(g->CheckBuildingPlacement(buildingPreview->GetPosition().x, buildingPreview->GetPosition().y, buildingPreview->GetSize()))
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

void Player::AddPreviewRoad(int x, int y)
{
    Game* g = Game::me;
    
    /*int amountNewR = roadPreviewHorizontal ? x - roadPreviews.back()->GetPosition().x : y - roadPreviews.back()->GetPosition().y;
    int increment = amountNewR >= 0 ? 1 : -1;
    /*amountNewR *= amountNewR >= 0 ? 1 : -1;

    printf(std::to_string(amountNewR).c_str());

    for(int i = amountNewR; i > 0; i--)
    {
        if(roadPreviewHorizontal)
        {
            auto road= new Road({x - (i * increment), y});
            roadPreviews.push_back(road);
            road->SetOutline(true);
        }
        else
        {
            auto road= new Road({x, y - (i * increment)});
            roadPreviews.push_back(road);
            road->SetOutline(true);
        }
    }*/

    

    auto road= new Road({x, y});
    roadPreviews.push_back(road);
    road->SetOutline(true);

    bool setRed = false;
    for(int i=0; i<roadPreviews.size(); i++)
    {
        if(setRed)
            roadPreviews[i]->SetOutlineColour(Color::Red);
        else
        {
            roadPreviews[i]->SetOutlineColour(Color::Green);

            if(!g->CheckRoadPlacement(roadPreviews[i]->GetPosition().x, roadPreviews[i]->GetPosition().y))
            {
                setRed = true;
                i = -1;
            }
        }
    }
}

void Player::RemovePreviewRoad(int x, int y)
{
    Game* g = Game::me;

    /*int amountNewR = roadPreviewHorizontal ? x - roadPreviews.back()->GetPosition().x : y - roadPreviews.back()->GetPosition().y;
    int increment = amountNewR >= 0 ? 1 : -1;
    amountNewR *= amountNewR >= 0 ? 1 : -1;

    for(int i = amountNewR; i > 0; i--)
    {
        if(roadPreviews.size() > 1)
        {
            delete roadPreviews.back();
            roadPreviews.pop_back();
        }
    }*/

    delete roadPreviews.back();
    roadPreviews.pop_back();
    
    bool setRed = false;
    for(int i=0; i<roadPreviews.size(); i++)
    {
        if(setRed)
            roadPreviews[i]->SetOutlineColour(Color::Red);
        else
        {
            roadPreviews[i]->SetOutlineColour(Color::Green);

            if(!g->CheckRoadPlacement(roadPreviews[i]->GetPosition().x, roadPreviews[i]->GetPosition().y))
            {
                setRed = true;
                i = -1;
            }
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

