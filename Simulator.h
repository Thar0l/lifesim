#pragma once

#include "libs.h"
#include "Settings.h"
#include "World.h"
#include <fstream>

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
	std::ofstream logfile;

	//TODO delete int variable and unite it to struct
	int frames;
	int fps;
	int dead;

	void Loop();
	void CheckEvents();
	void Draw();
	void SetTitle();

public:
	Simulator();
	void Init(sf::RenderWindow* window);
	void Run();

	~Simulator();
};

