#pragma once

#include "libs.h"
#include "Unit.h"


class World
{
private:
	sf::Vector2u size;
	sf::Image points;
	sf::RenderWindow* window;
	sf::Texture texture;
	sf::Sprite sprite;

	bool existPoint(int x, int y);

public:
	Settings* settings;
	std::list<Unit> units;

	World(sf::RenderWindow* window, int width, int height);
	void draw();
	~World();
	sf::Color getPoint(int x, int y);
	void setPoint(int x, int y, sf::Color color);
	int getUnitCount();
	sf::Vector2u getSize();
	void addCircle(int x, int y, int r, sf::Color color);
	void Clear();
};