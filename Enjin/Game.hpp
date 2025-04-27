#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"

class Road;
struct Material;
class Building;
using namespace sf;

class HotReloadShader;
class Entity;

class Game {

	std::vector<Building*> buildings;
	std::vector<Road*> roads;

	class Player* player;
	
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

	void PlaceRoad(int x, int y);
	bool TryDestroyRoad(int x, int y);
	
	void PlaceBuilding(int x, int y, Building* building);
	bool TryDestroyBuilding(int x, int y);

	bool CheckRoadPlacement(int x, int y);
	bool CheckBuildingPlacement(int x, int y, int size);

	void ProduceMaterial(Material& mat);

	void AddInhabitants(int amount);
};