#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"


using namespace sf;

class HotReloadShader;
class Entity;
class Game {
	
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
	void pollInput(double dt);

	void update(double dt);

	void draw(sf::RenderWindow& win);
	
	void im();
};