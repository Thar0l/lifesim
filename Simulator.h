#pragma once

#include "libs.h"
#include "Settings.h"
#include "World.h"

enum State
{
	st_run,
	st_pause,
	st_stop
};

struct DrawMode
{
	bool world;
	bool units;
};

struct Time
{
	sf::Time old;
	sf::Time current;
};

class Simulator
{
private:
	sf::RenderWindow* window;
	State state;
	DrawMode drawmode;
	Time time;
	Settings settings;
	World world;
	sf::Clock clock;

public:
	Simulator(sf::RenderWindow* window);
	void run();
	~Simulator();
};

