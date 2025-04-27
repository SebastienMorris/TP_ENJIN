#pragma once
#include <unordered_map>

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

    int nbBuildingTypes = 3;
    int buildingIndex = 0;

    bool placeRoad = false;

    Road* roadPreview;
    Building* buildingPreview;
    
public:
    Player();
    ~Player();
    
    void ProcessInput(sf::Event ev, sf::RenderWindow& win);
    void Update(double dt);
    void Draw(sf::RenderWindow& win);

    void Im();

    void AddResource(Materials resource, int amount);
    void AddInhabitants(int amount);

private:
    void UpdatePreviews(int x, int y);
    
    void Place(int x, int y);

    void PlaceRoadPreview(int x, int y);
    void PlaceBuildingPreview(int x, int y, Building* b);
    void ConfirmPlacement(int x, int y);
    
    void CheckMorale();
};
