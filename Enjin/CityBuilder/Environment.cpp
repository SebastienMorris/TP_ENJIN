#include <imgui.h>
#include "Environment.hpp"
#include "../HotReloadShader.hpp"


#include "../C.hpp"


Environment::Environment(sf::RenderWindow* win)
{
	this->win = win;
	if (useTmx) initTmxEnvironment();
	else initEnvironment();
	initBackground();
}

Environment::~Environment()
{
	delete bgShader;
	if (useTmx)
	{
		delete tmxZero;
		delete tmxMap;
	}
}



void Environment::initBackground()
{
	bool isOk = bgTexture.loadFromFile("res/bg_city.jpg");
	if (!isOk) printf("ERR : LOAD FAILED\n");
	bgHandle = new sf::RectangleShape(sf::Vector2f((float)win->getSize().x, (float)win->getSize().y));
	bgHandle->setTexture(&bgTexture);
	bgHandle->setSize(sf::Vector2f(C::SCREEN_WIDTH, C::SCREEN_HEIGHT));
	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
}

void Environment::initEnvironment()
{
	throw std::exception(); // Will not be implemented cause using tmx instead
}

void Environment::initTmxEnvironment()
{
	// Init Tmx Tilemap
	tmxMap = new tmx::Map();
	tmxMap->load("Assets/ENJIN_TileMap.tmx");
	tmxZero = new MapLayer(*tmxMap, 0);
	//tmxOne = new MapLayer(*tmxMap, 1);
	
	// Register Node Locations
	auto tileCount = tmxMap->getTileCount();
	for (int x = 0; x < (int)tileCount.x; ++x) {
		for (int y = 0; y < (int)tileCount.y; ++y) {
#pragma warning( push )
#pragma warning( disable : 26813)
			if (getWater(x, y)) water.push_back({x, y});
#pragma warning( pop )
		}
	}
}



void Environment::update(double dt)
{
	if (bgShader) bgShader->update(dt);
	tmxZero->update(sf::seconds((float)dt));
}



void Environment::drawWorld(sf::RenderTarget& win)
{
	sf::RenderStates states = sf::RenderStates::Default;
	return;
}

void Environment::drawCamera(sf::RenderTarget& win)
{
	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader* sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &bgTexture;
	sh->setUniform("texture", bgTexture);
	win.draw(*bgHandle, states);
	if (useTmx)
	{
		win.draw(*tmxZero, sf::RenderStates::Default);
	}
}

void Environment::imgui()
{
	using namespace ImGui;
}

bool Environment::isWater(int x, int y)
{
	for(auto w : water)
		if(w.x == x && w.y == y) return true;

	return false;
}

bool Environment::isWater(int x, int y, int size)
{
	for(auto w : water)
	{
		bool xCheck = w.x <= x + (size - 1) / 2 && w.x >= x - (size - 1) / 2;
		bool yCheck = w.y <= y + (size - 1) / 2 && w.y >= y - (size - 1) / 2;
		if(xCheck && yCheck) return true;
	}

	return false;
}

bool Environment::getWater(int x, int y)
{
	uint32_t id = tmxZero->getTile(x, y).ID;
	if (mapCache.find(id) != mapCache.end()) {
		return mapCache.at(id);
	}
	
	// If no cache, process to a search and resolution
	for (auto tileset : tmxMap->getTilesets()) {
		if (tileset.hasTile(id)) {
			std::string tileType = tileset.getTile(id)->className;
			if (tileType == "Water") {
				mapCache.insert({ id, 1 });
				return true;
			}
		}
	}

	// Return None if no resolution possible
	mapCache.insert({ id, 0 });
	return false;
}