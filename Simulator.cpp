#include "Simulator.h"
#include "libs.h"

void srand()
{
	//
}

/**************************************************************************************************/ 

Simulator::Simulator()
{
}

/**************************************************************************************************/

void Simulator::Init(sf::RenderWindow* window)
{
	this->settings.init("settings.ini");

	this->window = window;
	this->window->create(sf::VideoMode(settings.size_x, settings.size_y), "LifeSim");
	this->window->setVerticalSyncEnabled(false);
	this->window->setFramerateLimit(0);
	this->window->setPosition(sf::Vector2i(0, 0));

	this->state = st_run;

	this->drawmode.units = true;
	this->drawmode.world = true;

	this->time.old = sf::Time::Zero;
	this->time.current = sf::Time::Zero;

	this->world.Init(window, &settings, settings.size_x, settings.size_y);
	this->world.FillCircles();

	this->logfile.open("log.txt");

	this->frames = 0;
	this->fps = 0;

	this->dead = 0;
}

/**************************************************************************************************/

void Simulator::Run()
{
	srand();
	clock.restart();
	while (window->isOpen())
	{
		this->Loop();
	}
	logfile.close();
}

/**************************************************************************************************/

void Simulator::Loop()
{
	this->CheckEvents();

	if (state == st_run)
	{
		if (world.HasUnits())
		{
			std::list<Unit>::iterator unit = world.units.begin();
			while (unit != world.units.end())
			{
				unit->Live();
				unit++;
			}
		}
		else
		{
			state = st_stop;
		}
		time.current = clock.getElapsedTime();
        //FIXME Time continue counting in pause mode
	}

	if (((int)(time.current.asSeconds())) > (int)(time.old.asSeconds()))
	{
		time.old = time.current;
		fps = frames;
		frames = 0;
		logfile << (int)(time.current.asSeconds()) << "; " << world.GetUnitCount() << "; " << dead << std::endl;
	}

	this->SetTitle();
	this->Draw();
	frames++;
}

/**************************************************************************************************/

void Simulator::CheckEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::V)
			{
				drawmode.units = !drawmode.units;
			}
			if (event.key.code == sf::Keyboard::P)
			{
				if (state == st_run)
					state = st_pause;
				else if (state == st_pause)
					state = st_run;
			}
			if (event.key.code == sf::Keyboard::C)
			{
				world.Clear();
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}
		}
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
	}
}

/**************************************************************************************************/

void Simulator::Draw()
{
	window->clear();
	world.Draw();

	if (world.HasUnits())
	{
		std::list<Unit>::iterator unit = world.units.begin();
		while (unit != world.units.end())
		{
			if (unit->Alive())
			{
				if (drawmode.units)
					unit->Draw();
				unit++;
			}
			else
			{
				dead++;
				if (unit != world.units.end())
					unit = world.units.erase(unit);
				else world.units.pop_back();
			}
		}
	}
	window->display();
}

/**************************************************************************************************/

void Simulator::SetTitle()
{
	std::stringstream title;
	//TODO add dead units count and fix age[0] to maxage
	title << "Units: " << world.GetUnitCount() << "; Died: " << dead << "; Time: " << (int)(time.current.asSeconds()) << "s; FPS: " << fps;
	window->setTitle(sf::String(title.str()));
}

/**************************************************************************************************/

Simulator::~Simulator()
{
}
