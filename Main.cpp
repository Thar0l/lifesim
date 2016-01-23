#define _CRT_SECURE_NO_WARNINGS
#include "libs.h"
#include "World.h"
#include "Unit.h"
#include "Settings.h"
#include "Simulator.h"
#include <fstream>

int main()
{
	srand(time(NULL));
	sf::RenderWindow window;
	Simulator sim;
	sim.Init(&window);
	sim.Run();
	return 0;
}

/*TODO list*/
/***********
Predators
Menu
************/
