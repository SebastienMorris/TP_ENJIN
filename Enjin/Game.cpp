
#include <imgui.h>
#include <array>

#include "C.hpp"
#include "Game.hpp"

#include <fstream>

#include "HotReloadShader.hpp"
#include "CityBuilder/Building.h"
#include "CityBuilder/Player.h"


Game* Game::me = 0;

static int cols = C::SCREEN_WIDTH / C::GRID_SIZE;
static int lastLine = C::SCREEN_HEIGHT / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {
	this->win = win;
	
	me = this;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::SCREEN_WIDTH, C::SCREEN_HEIGHT));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	player = new Player();
}

Game::~Game()
{
	delete player;
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}

	player->ProcessInput(ev);
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	g_time += dt;
	if (bgShader) bgShader->update(dt);
	
	beforeParts.update(dt);

	player->Update(dt);

	for(auto b : buildings)
		b->Update(dt);
	
	afterParts.update(dt);
}

 void Game::draw(sf::RenderWindow& win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for(auto b : buildings)
		b->Draw(win);

	afterParts.draw(win);
}

void Game::im()
{
	
}

bool Game::TryPlaceBuilding(int x, int y)
{
	for(auto b : buildings)
		if(b->GetPosition().x == x && b->GetPosition().y == y)
			return false;

	//Building object creation and add to array

	return true;
}

bool Game::TryDestroyBuilding(int x, int y)
{
	for(auto b : buildings)
	{
		if(b->GetPosition().x == x && b->GetPosition().y == y)
		{
			//Destroy object and remove from array
			return true;
		}
	}

	return false;
}






