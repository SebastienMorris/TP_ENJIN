#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"
#include "ParticleMan.hpp"

class Road;
struct Material;
class Building;
using namespace sf;

class HotReloadShader;

class Game {

	std::vector<Building*> buildings;
	std::vector<Road*> roads;

	class Player* player;

	class Environment* map;
	
public:
	
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;


	ParticleMan beforeParts;
	ParticleMan afterParts;

	static Game* me;

	Game(sf::RenderWindow * win);
	~Game();

	void processInput(sf::Event ev);

	void update(double dt);

	void draw(sf::RenderWindow& win);
	
	void im();

	void PlaceRoad(Road* road);
	bool TryDestroyRoad(int x, int y);
	
	void PlaceBuilding(Building* building);
	bool TryDestroyBuilding(int x, int y);

	bool CheckRoadPlacement(int x, int y);
	bool CheckBuildingPlacement(int x, int y, int size);

	void ProduceMaterial(Material& mat);

	void AddInhabitants(int amount);
};