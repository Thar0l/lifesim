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
	sf::Color resist;
	bool alive;
	
	int food;
	int health;
	int energy;
	int speed;
	int size;
	int age;

	void Move(direction dir);
	void Split();
	void Growth();

	void Eat();
	void Fill();
	



public:
	Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res);
	void Draw();
	int GetAge();
	void Live();
	bool Alive();
	~Unit();
};