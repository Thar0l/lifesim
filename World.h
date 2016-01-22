#pragma once

#include "libs.h"
#include "Unit.h"


class World
{
private:
	sf::Vector2u size;
	sf::Image points;
	sf::RenderWindow* window;
	Settings* settings;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Shader shader;

	bool existPoint(int x, int y);

public:
	int died;
	std::list<Unit> units;
	World();
	void init(sf::RenderWindow* window, Settings* settings, int width, int height);
	void draw();
	~World();
	sf::Color getPoint(int x, int y);
	void setPoint(int x, int y, sf::Color color);
	int getUnitCount();
	sf::Vector2u getSize();
	void addCircle(int x, int y, int r, sf::Color color);
	void Clear();
};