#pragma once

#include "libs.h"
#include "World.h"

struct HsvColor
{
	int h;
	int s;
	int v;
};

sf::Color inv(sf::Color color);
HsvColor RgbToHsv(sf::Color rgb);
sf::Color HsvToRgb(HsvColor hsv);

class Unit
{
private:
	sf::RenderWindow* window;
	Settings* settings;
	World* world;

	sf::CircleShape image;
	sf::Color resist;

	bool alive;
	
	int food;
	int speed;
	int size;
	int age;
	int generation;

	void Move(direction dir);
	void Split();
	void Growth();
	void Eat();
	void Fill();

public:
	Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res, int generation = 0);
	void Draw();
	int GetAge();
	int GetGeneration();
	void Live();
	bool Alive();
	~Unit();
};