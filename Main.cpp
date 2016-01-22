#define _CRT_SECURE_NO_WARNINGS
#include "libs.h"
#include "World.h"
#include "Unit.h"
#include "Settings.h"
#include "Simulator.h"
#include <fstream>

#define empty_mode 0
#define circle_mode 1
#define unit_mode 2

void srand()
{
	srand(time(NULL));
}

int main()
{
	sf::RenderWindow window;
	Simulator sim(&window);
	sim.run();
	return 0;
}
