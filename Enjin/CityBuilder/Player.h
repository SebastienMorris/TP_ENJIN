#pragma once
#include <unordered_map>
#include <SFML/System/Vector3.hpp>

#include "Building.h"

class Road;
namespace sf
{
    class Event;
}

class Player
{
    std::unordered_map<Materials, Material*> inventory = {
        {Materials::MONEY, new Material(Materials::MONEY, 10)},
        {Materials::WOOD, new Material(Materials::WOOD, 10)},
        {Materials::ELECTRICITY, new Material(Materials::ELECTRICITY, 10)}
    };

    float morale = 0.0f;
    float moraleCheckFrequency = 5.0f;
    float moraleCheckTimer = 0.0f;
    
    int population = 0;

    int nbBuildingTypes = 4;
    int buildingIndex = 0;

    std::vector<Road*> roadPreviews;
    Building* buildingPreview;

    sf::Vector3f buildingSnapSlots[4] = {
        {1, 0, 0.0f},
        {-1, 0, 0.0f},
        {0, -1, 0.0f},
        {0, 1, 0.0f},
    };

    bool roadPreviewHorizontal = true;
    
public:
    Player();
    ~Player();
    
    void ProcessInput(sf::Event ev, sf::RenderWindow& win);
    void Update(double dt);
    void Draw(sf::RenderWindow& win);

    void Im();

    void ProcessCost(Materials resource, int amount);
    void AddResource(Materials resource, int amount);
    void AddInhabitants(int amount);

private:
    void UpdatePreviews(int x, int y);
    sf::Vector2i TrySnapBuilding(int mouseX, int mouseY, Building* building);
    //sf::Vector2i TrySnapRoad(int mouseX, int mouseY, Building* );
    
    void Place(int x, int y);

    bool CheckCanPlace(int x, int y, Building* b);
    void PlaceRoadPreview(int x, int y);
    void PlaceBuildingPreview(int x, int y, Building* b);
    void ConfirmPlacement();

    void AddPreviewRoad(int x, int y);
    void RemovePreviewRoad(int x, int y);
    
    void CheckMorale();
};
