
#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include "CityBuilder/Building.h"
#include "CityBuilder/Environment.hpp"
#include "CityBuilder/Player.h"
#include "CityBuilder/Road.h"


Game* Game::me = 0;

static int cols = C::SCREEN_WIDTH / C::GRID_SIZE;
static int lastLine = C::SCREEN_HEIGHT / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {
	this->win = win;
	
	me = this;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	map = new Environment(win); 

	/*bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}*/
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

	player->ProcessInput(ev, *win);
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

	map->drawCamera(win);

	for(auto b : buildings)
		b->Draw(win);

	for(auto r : roads)
		r->Draw(win);

	player->Draw(win);

	afterParts.draw(win);
}

void Game::im()
{
	player->Im();
}

void Game::PlaceRoad(Road* road)
{
	if(!road) return;
	
	roads.push_back(road);
}

bool Game::TryDestroyRoad(int x, int y)
{
	Road* destroyR = nullptr;
	for(auto r : roads)
	{
		if(r->GetPosition().x == x && r->GetPosition().y == y)
		{
			//Destroy object and remove from array
			destroyR = r;
			break;
		}
	}

	if(destroyR)
	{
		auto it = std::find(roads.begin(), roads.end(), destroyR);
		roads.erase(it);
		delete destroyR;
		return true;
	}
	return false;
}


void Game::PlaceBuilding(Building* building)
{
	if(!building) return;
	
	buildings.push_back(building);
}

bool Game::TryDestroyBuilding(int x, int y)
{
	Building* destroyB = nullptr;
	for(auto b : buildings)
	{
		bool xCheck = x <= b->GetPosition().x + b->GetSize() && x >= b->GetPosition().x - b->GetSize();
		bool yCheck = y <= b->GetPosition().y + b->GetSize() && y >= b->GetPosition().y - b->GetSize();
		
		if(xCheck && yCheck)
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

bool Game::CheckRoadPlacement(int x, int y)
{
	if(map->isWater(x, y)) return false;
	
	for(auto r : roads)
	{
		if(x == r->GetPosition().x && y == r->GetPosition().y) return false;
	}
	
	for(auto b : buildings)
	{
		bool xCheck = x <= b->GetPosition().x + (b->GetSize() + 1) / 2 - 1 && x >= b->GetPosition().x - (b->GetSize() + 1) / 2 + 1;
		bool yCheck = y <= b->GetPosition().y + (b->GetSize() + 1) / 2 - 1 && y >= b->GetPosition().y - (b->GetSize() + 1) / 2 + 1;
		
		if(xCheck && yCheck) return false;
	}
	return true;
}

bool Game::CheckRoadAround(int x, int y, int size)
{
	for(auto r : roads)
	{
		bool xCheckR = x + (size - 1) / 2 <= r->GetPosition().x + 1 && x + (size - 1) / 2 >= r->GetPosition().x - 1;
		bool xCheckL = x - (size - 1) / 2 <= r->GetPosition().x + 1 && x - (size - 1) / 2 >= r->GetPosition().x - 1;
		bool yCheckR = y + (size - 1) / 2 <= r->GetPosition().y + 1 && y + (size - 1) / 2 >= r->GetPosition().y - 1;
		bool yCheckL = y - (size - 1) / 2 <= r->GetPosition().y + 1 && y - (size - 1) / 2 >= r->GetPosition().y - 1;
		
		if((xCheckR || xCheckL) && (yCheckR || yCheckL)) return true;
	}
	return false;
}

bool Game::CheckBuildingPlacement(int x, int y, int size)
{
	if(map->isWater(x, y, size)) return false;
	
	for(auto b : buildings)
	{
		bool xCheckR = x + (size - 1) / 2 <= b->GetPosition().x + (b->GetSize() - 1) / 2 + 1 && x + (size - 1) / 2 >= b->GetPosition().x - (b->GetSize() - 1) / 2 - 1;
		bool xCheckL = x - (size - 1) / 2 <= b->GetPosition().x + (b->GetSize() - 1) / 2 + 1 && x - (size - 1) / 2 >= b->GetPosition().x - (b->GetSize() - 1) / 2 - 1;
		bool yCheckR = y + (size - 1) / 2 <= b->GetPosition().y + (b->GetSize() - 1) / 2 + 1 && y + (size - 1) / 2 >= b->GetPosition().y - (b->GetSize() - 1) / 2 - 1;
		bool yCheckL = y - (size - 1) / 2 <= b->GetPosition().y + (b->GetSize() - 1) / 2 + 1 && y - (size - 1) / 2 >= b->GetPosition().y - (b->GetSize() - 1) / 2 - 1;
		
		if((xCheckR || xCheckL) && (yCheckR || yCheckL)) return false;
	}
	return true;
}

void Game::ProduceMaterial(Material& mat)
{
	int newAmout = player->GetMorale() < 0 ? mat.amount / 2 :  mat.amount;
	player->AddResource(mat.type, newAmout);
}

void Game::AddInhabitants(int amount)
{
	player->AddInhabitants(amount);
}






