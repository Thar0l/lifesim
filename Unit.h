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
int sign(int x);

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
	long long unsigned int id;
	bool predator;
	std::list<Unit>::iterator victim;

	sf::Vector2f aim;

	void Move(direction dir);
	void Split();
	void Growth();
	void Eat();
	void Fill();

public:
	Unit(sf::RenderWindow* window, World* world, Settings* settings, int size, int x, int y, sf::Color res, int generation = 0);
	void Draw();
	int GetAge();
	int GetGeneration();
	int getDistance(sf::Vector2f target);
	void Live();
	bool Alive();
	bool Predator();
	int BeEaten();
	sf::Vector2f GetPosition();

	void MakePredator();

	long long unsigned int GetID();
	~Unit();
};