#define _CRT_SECURE_NO_WARNINGS
#include "libs.h"
#include "World.h"
#include "Unit.h"
#include "Settings.h"

void srand()
{
	srand(time(NULL));
}

int main()
{
	bool stop = false;
	bool pause = false;
	sf::Time time = sf::Time::Zero;
	sf::Time oldtime = sf::Time::Zero;
	int frames = 0;
	int fps = 0;

	Settings settings("settings.ini");
	sf::RenderWindow window(sf::VideoMode(settings.size_x, settings.size_y), "SFML works!");
	World world(&window, &settings, settings.size_x, settings.size_y);

	sf::Clock clock;

	srand();
	

	
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);

	bool candraw = true;

	
	
	
	
	for (int i = 0; i < (rand() % 200 + 40); i++)
	{
		int t = rand() % 3;
		int f = rand() % 200;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 160 + 80, sf::Color(r, g, b));
	}
	for (int i = 0; i < (rand() % 300 + 50); i++)
	{
		int t = rand() % 3;
		int f = 255;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 100 + 120, sf::Color(r, g, b));
	}
	for (int i = 0; i < (rand() % 80 + 30); i++)
	{
		int t = rand() % 3;
		int f = rand() % 160;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 320 + 220, sf::Color(r, g, b));
	}
	clock.restart();
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
			{
				if (candraw) 
					candraw = false;
				else
					candraw = true;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				if (pause)
					pause = false;
				else
					pause = true;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(rand() % 120 + 100, 0, 0));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
			{
				world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(0, rand() % 120 + 100, 0));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
			{
				world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(0, 0, rand() % 120 + 100));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
			{
				world.Clear();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
			{
				for (int i = 0; i < 8; i++)
				{
					sf::Color resist;
					resist.b = 0;
					if (i % 4 == 0)
					{
						resist.r = 240;
						resist.g = 20;
					}
					else if (i % 4 == 1)
					{
						resist.r = 20;
						resist.g = 240;
					}
					else if (i % 4 == 2)
					{
						resist.r = 220;
						resist.g = 220;
					}
					else
					{
						resist.r = 20;
						resist.g = 20;
					}

					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(settings.size_x), rand() % (int)(settings.size_y), resist));
				}
			}
			

		}

		if ( (!pause) && (world.units.size() > 0))
		{
			std::list<Unit>::iterator it = world.units.begin();
			while (it != world.units.end())
			{
				it->live();
				it++;
			}
		}

		if (!stop) 
			time = clock.getElapsedTime();

		frames++;
		if ( (int)(time.asSeconds()) != (int)(oldtime.asSeconds()))
		{
			oldtime = time;
			fps = frames;
			frames = 0;
		}
		if (world.getUnitCount() == 0) stop = true;


		std::stringstream title;
		title << "Units: " << world.getUnitCount() << "; Time: " << (int)(time.asSeconds()) << "s; FPS: " << fps;
		window.setTitle(sf::String(title.str()));

		window.clear();
		world.draw();

		if (world.units.size() > 0)
		{
			std::list<Unit>::iterator unit = world.units.begin();
			while (unit != world.units.end())
			{
				if (unit->isAlive())
				{
					if (candraw)
						unit->draw();
					unit++;
				}
				else
				{
					if (unit != world.units.end())
						unit = world.units.erase(unit);
					else world.units.pop_back();
				}
			}
		}
		window.display();
	}

	return 0;
}
