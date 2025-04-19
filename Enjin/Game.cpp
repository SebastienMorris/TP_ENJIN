
#include <imgui.h>
#include <array>

#include "C.hpp"
#include "Game.hpp"

#include <fstream>

#include "HotReloadShader.hpp"
#include "CityBuilder/Building.h"
#include "CityBuilder/House.h"
#include "CityBuilder/Player.h"
#include "CityBuilder/Road.h"


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

	for(auto r : roads)
		r->Update(dt);

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

	for(auto r : roads)
		r->Draw(win);

	afterParts.draw(win);
}

void Game::im()
{
	
}


bool Game::TryPlaceBuilding(int x, int y, Building* building)
{
	for(auto b : buildings)
	{
		bool xCheck = x <= b->GetPosition().x + b->GetSize() && x >= b->GetPosition().x - b->GetSize();
		bool yCheck = y <= b->GetPosition().y + b->GetSize() && y >= b->GetPosition().y - b->GetSize();
		
		if(xCheck && yCheck) return false;
	}
	
	buildings.push_back(building);
	building->SetPosition(x, y);

	return true;
}

bool Game::TryDestroyBuilding(int x, int y)
{
	Building* destroyB = nullptr;
	for(auto b : buildings)
	{
		if(b->GetPosition().x == x && b->GetPosition().y == y)
		{
			//Destroy object and remove from array
			destroyB = b;
			break;
		}
	}

	if(destroyB)
	{
		auto it = std::find(buildings.begin(), buildings.end(), destroyB);
		buildings.erase(it);
		delete destroyB;
		return true;
	}
	return false;
}

void Game::ProduceMaterial(Material& mat)
{
	switch (mat.type)
	{
	case Materials::MONEY:
		player->AddWood(mat.amount);
		break;

	case Materials::WOOD:
		player->AddWood(mat.amount);
		break;

	case Materials::ELECTRICITY:
		player->AddElectricity(mat.amount);
		break;
	}
}






