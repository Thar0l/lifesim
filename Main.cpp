#define _CRT_SECURE_NO_WARNINGS
#include "libs.h"
#include "World.h"
#include "Unit.h"
#include "Settings.h"

int main()
{
	int sizex = 1904;
	int sizey = 1000;
	float x = 100.0;
	float y = 100.0;
	float d = 5.0;
	bool stop = false;
	bool pause = false;
	int time_ = 0;
	int oldtime_ = 0;
	int indexi = 0;
	int indexj = 1000;
	int frames = 0;
	int fps = 0;
	sf::RenderWindow window(sf::VideoMode(sizex, sizey), "SFML works!");
	sf::Clock clock;
	sf::Clock timeclock;

	
	window.setFramerateLimit(0);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(40);
	srand(time(NULL));

	bool candraw = true;

	
	Settings settings("settings.ini");
	World world(&window, &settings, sizex, sizey);
	
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
		world.addCircle(rand() % sizex, rand() % sizex, rand() % 160 + 80, sf::Color(r, g, b));
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
		world.addCircle(rand() % sizex, rand() % sizex, rand() % 100 + 120, sf::Color(r, g, b));
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
		world.addCircle(rand() % sizex, rand() % sizex, rand() % 320 + 220, sf::Color(r, g, b));
	}
	clock.restart();
	while (window.isOpen())
	{

		sf::Event event;
		sf::Time elapsed = clock.restart();
		if (!pause)
			if (world.units.size() > 0)
			{
				std::list<Unit>::iterator it = world.units.begin();
				while (it != world.units.end())
				{
					it->live();
					it++;
				}
			}



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
				world.addCircle(rand() % sizex, rand() % sizex, rand() % 360 + 160, sf::Color(rand() % 120 + 100, 0, 0));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
			{
				world.addCircle(rand() % sizex, rand() % sizex, rand() % 360 + 160, sf::Color(0, rand() % 120 + 100, 0));
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
			{
				world.addCircle(rand() % sizex, rand() % sizex, rand() % 360 + 160, sf::Color(0, 0, rand() % 120 + 100));
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

					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(sizex), rand() % (int)(sizey), resist));
				}
			}
			

		}

		sf::Time elapsed1;
		if (!stop) 
		{ 
			elapsed1 = timeclock.getElapsedTime(); 
		}
		time_ = elapsed1.asSeconds();

		frames ++;
		if (time_ != oldtime_)
		{
			oldtime_ = time_;
			fps = frames;
			frames = 0;
		}
		if (world.getUnitCount() == 0) stop = true;

		char* str2 = new char[10];
		char* str3 = new char[10];
		char* str4 = new char[10];
		_itoa(world.getUnitCount(), str2, 10);
		_itoa(time_, str3, 10);
		_itoa(fps, str4, 10);
		window.setTitle(sf::String("Units: ") + sf::String(str2) + sf::String(";  Time: ") + sf::String(str3) + sf::String(" s.  Framerate: ") + sf::String(str4) + sf::String(" FPS."));

		window.clear();
		world.draw();

		if (world.units.size() > 0)
		{
			std::list<Unit>::iterator it = world.units.begin();
			while (it != world.units.end())
			{
				if (it->isAlive())
				{
					if (candraw)
						it->draw();
					it++;
				}
				else
				{
					if (it != world.units.end())
						it = world.units.erase(it);
					else world.units.pop_back();
				}
			}
		}
		window.display();

	}

	return 0;
}
