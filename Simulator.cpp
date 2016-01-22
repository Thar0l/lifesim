#include "Simulator.h"
#include "libs.h"


Simulator::Simulator(sf::RenderWindow* window)
{
	this->window = window;
	this->state = st_run;
	this->drawmode.units = true;
	this->drawmode.world = true;
	this->time.old = sf::Time::Zero;
	this->time.current = sf::Time::Zero;
	this->settings.init("settings.ini");
	this->window->create(sf::VideoMode(settings.size_x, settings.size_y), "LifeSim");
	this->world.init(window, &settings, settings.size_x, settings.size_y);
	sf::Clock clock;
}


void Simulator::run()
{
	int frames = 0;
	int fps = 0;
	srand(0);
	window->setVerticalSyncEnabled(false);
	window->setFramerateLimit(0);
	window->setPosition(sf::Vector2i(0, 0));

	for (int i = 0; i < (rand() % 120 + 25); i++)
	{
		int t = i;
		int f = 255;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 200 + 80, sf::Color(r, g, b));
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
	std::ofstream myfile;
	myfile.open("log.txt");


	while (window->isOpen())
	{

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window->close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
			{
				if (drawmode.units)
					drawmode.units = false;
				else
					drawmode.units = true;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				if (state == st_run)
					state = st_pause;
				else
					state = st_run;
			}/*
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				if (mode == circle_mode)
					world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(rand() % 120 + 100, 0, 0));
				if (mode == unit_mode)
					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(settings.size_x), rand() % (int)(settings.size_y), sf::Color(255, 0, 0)));
				mode = empty_mode;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
			{
				if (mode == circle_mode)
					world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(0, rand() % 120 + 100, 0));
				if (mode == unit_mode)
					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(settings.size_x), rand() % (int)(settings.size_y), sf::Color(0, 255, 0)));
				mode = empty_mode;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
			{
				if (mode == circle_mode)
					world.addCircle(rand() % settings.size_x, rand() % settings.size_x, rand() % 360 + 160, sf::Color(0, 0, rand() % 120 + 100));
				if (mode == unit_mode)
					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(settings.size_x), rand() % (int)(settings.size_y), sf::Color(0, 0, 255)));
				mode = empty_mode;
			}*/
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
			{
				world.Clear();
			}/*
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)
			{
				mode = unit_mode;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
			{
				mode = circle_mode;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
			{
				for (int i = 0; i < 12; i++)
				{
					sf::Color resist;
					if (i % 6 == 0)
					{
						resist.r = 255;
						resist.g = 0;
						resist.b = 0;
					}
					else if (i % 6 == 1)
					{
						resist.r = 0;
						resist.g = 255;
						resist.b = 0;
					}
					else if (i % 6 == 2)
					{
						resist.r = 0;
						resist.g = 0;
						resist.b = 255;
					}
					else if (i % 6 == 3)
					{
						resist.r = 255;
						resist.g = 0;
						resist.b = 255;
					}
					else if (i % 6 == 4)
					{
						resist.r = 255;
						resist.g = 255;
						resist.b = 0;
					}
					else
					{
						resist.r = 255;
						resist.g = 255;
						resist.b = 0;
					}

					world.units.push_back(Unit(&window, &world, &settings, rand() % (int)(settings.size_x), rand() % (int)(settings.size_y), resist));
				}
			}*/


		}

		if ((state != st_pause) && (world.units.size() > 0))
		{
			std::list<Unit>::iterator it = world.units.begin();
			while (it != world.units.end())
			{
				it->live();
				it++;
			}
		}

		if (state != st_stop)
			time.current = clock.getElapsedTime();

		frames++;
		if ((int)(time.current.asSeconds()) != (int)(time.old.asSeconds()))
		{
			time.old = time.current;
			fps = frames;
			frames = 0;
			myfile << world.getUnitCount() << "\n";
		}
		if (world.getUnitCount() == 0) state = st_stop;


		std::stringstream title;
		title << "Units: " << world.getUnitCount() << "; Died: " << world.died << "; Time: " << (int)(time.current.asSeconds()) << "s; FPS: " << fps;
		window->setTitle(sf::String(title.str()));

		window->clear();
		world.draw();

		if (world.units.size() > 0)
		{
			std::list<Unit>::iterator unit = world.units.begin();
			while (unit != world.units.end())
			{
				if (unit->isAlive())
				{
					if (drawmode.units)
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
		window->display();
	}
	myfile.close();
}


Simulator::~Simulator()
{
}
