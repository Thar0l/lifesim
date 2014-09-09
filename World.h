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
	sf::Shader shader;
	sf::Sprite sprite;
	sf::RenderTexture render_texture;

	int tmp;

	bool existPoint(int x, int y);

public:
	Settings* settings;
	std::list<Unit> units;

	World(sf::RenderWindow* window, int width, int height);
	void draw();
	~World();

	sf::Color getPoint(int x, int y);
	void setPoint(int x, int y, sf::Color color);
	sf::VertexArray _draw_getPoints();
	int getUnitCount();
	sf::Vector2u getSize();

	void addCircle(int x, int y, int r, int cr, int cg, int cb);
	void Clear();
	void Blur(int i);
	void BlurBlock(int i, int j);
	void BlurPoint(int i, int j);
};