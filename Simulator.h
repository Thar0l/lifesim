#pragma once

#include "libs.h"
#include "Settings.h"
#include "World.h"
#include <fstream>
#include <ctime>
#include "Menu.h"
#include "Unit.h"

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
	bool menu;
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
	Time simtime;
	Settings settings;
	World world;
	sf::Clock clock;
	std::ofstream logfile;

	Menu menu;

	//TODO delete int variable and unite it to struct
	int frames;
	int fps;
	int dead;
	int index;
	int eldest;
	int youngest;

	sf::Color mousecolor;
	HsvColor hsvmousecolor;
	sf::Color unitcolors[6];
	int unitcolorindex;

	std::string timestamp;

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

