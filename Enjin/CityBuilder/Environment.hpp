#pragma once


#include <../tmxlite-1.4.4/include/tmxlite/Map.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../SFMLOrthogonalLayer.hpp"
//#include "../HotReloadShader.hpp"

//using namespace sf;


namespace sf
{
	class RectangleShape;
}

class Environment
{
private:
	sf::RenderWindow* win;

	class HotReloadShader* bgShader = nullptr;
	sf::Texture	bgTexture;
	sf::RectangleShape* bgHandle;

	const bool useTmx = true;
	tmx::Map* tmxMap = nullptr;
	MapLayer* tmxZero = nullptr;
	//MapLayer* tmxOne = nullptr;

	std::map<uint32_t, uint32_t> mapCache;

public:
	std::vector<sf::Vector2i> water;

	~Environment();
	Environment(sf::RenderWindow* win);
	void initBackground();
	void initEnvironment();
	void initTmxEnvironment();

	void update(double dt);
	void drawWorld(sf::RenderTarget& win);
	void drawCamera(sf::RenderTarget& win);
	void imgui();

	bool isWater(int x, int y);
	bool isWater(int x, int y, int size);

private:
	bool getWater(int x, int y);
};
