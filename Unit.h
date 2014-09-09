#pragma once

#include "libs.h"
#include "World.h"

class Unit
{
private:
	sf::RenderWindow* window;
	sf::CircleShape image;
	World* world;
	bool alive;
	sf::Color resist;
	int food;
	int health;
	int energy;
	int visiblity;
	int speed;
	int size;

	void move(direction dir);
	bool eat();
	void split();
	void growth();


public:
	Unit(sf::RenderWindow* window, World* world, int x, int y, sf::Color res);
	void draw();
	direction searchFood(int r);
	~Unit();
	void live();
	bool isAlive();
};