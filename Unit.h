#pragma once

#include "libs.h"
#include "World.h"

class Unit
{
private:
	sf::RenderWindow* window;
	Settings* settings;
	World* world;
	sf::CircleShape image;
	bool alive;
	sf::Color resist;
	int food;
	int health;
	int energy;
	int visiblity;
	int speed;
	int size;

	void move(direction dir);
	bool _eat();
	void split();
	void growth();

	void eat();
	void fill();
	



public:
	Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res);
	void draw();
	direction searchFood(int r);
	~Unit();
	void live();
	bool isAlive();
};