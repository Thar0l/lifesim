#include "Simulator.h"
#include "libs.h"

std::string ColorToStr(sf::Color color)
{
	std::string result = "N/A";
	if (color == sf::Color(255, 0, 0)) result = "Red";
	if (color == sf::Color(0, 255, 0)) result = "Green";
	if (color == sf::Color(0, 0, 255)) result = "Blue";
	if (color == sf::Color(255, 255, 0)) result = "Yellow";
	if (color == sf::Color(0, 255, 255)) result = "Cyan";
	if (color == sf::Color(255, 0, 255)) result = "Magenta";
	return result;
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
	this->drawmode.menu  = false;

	this->simtime.old = sf::Time::Zero;
	this->simtime.current = sf::Time::Zero;

	this->world.Init(window, &settings, settings.size_x, settings.size_y);
	this->world.FillCircles();

	this->frames = 0;
	this->fps = 0;

	this->dead = 0;
	this->index = 0;
	this->eldest = MAXINT;
	this->youngest = 0;

	this->hsvmousecolor.h = 120;
	this->hsvmousecolor.s = 255;
	this->hsvmousecolor.v = 255;

	this->mousecolor = HsvToRgb(hsvmousecolor);
	this->unitcolors[0] = sf::Color(255, 0, 0);
	this->unitcolors[1] = sf::Color(0, 255, 0);
	this->unitcolors[2] = sf::Color(0, 0, 255);
	this->unitcolors[3] = sf::Color(255, 255, 0);
	this->unitcolors[4] = sf::Color(0, 255, 255);
	this->unitcolors[5] = sf::Color(255, 0, 255);
	this->unitcolorindex = 0;

	this->menu.Init(window);
	this->menu.SetPosition(sf::Vector2f(2, 3));
	this->menu.SetMouseColor(mousecolor);

	time_t t = time(NULL);
	tm* now = new tm;
	localtime_s(now, &t);
	std::stringstream ss;
	ss << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday << '-' << now->tm_hour << '-' << now->tm_min << '-' << now->tm_sec;
	this->timestamp = ss.str();
	std::cout << "Simulation id: " << timestamp << std::endl;

	this->logfile.open("logs\\" + timestamp + "_log.txt");
}

/**************************************************************************************************/

void Simulator::Run()
{
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
			this->eldest = MAXINT;
			this->youngest = 0;
			std::list<Unit>::iterator unit = world.units.begin();
			while (unit != world.units.end())
			{
				unit->Live();
				if (unit->GetGeneration() < eldest)
				{
					eldest = unit->GetGeneration();
				}
				if (unit->GetGeneration() > youngest)
				{
					youngest = unit->GetGeneration();
				}
				unit++;
			}
		}
		else
		{
			state = st_stop;
			if (settings.screenshot_delay != 0)
			{
				sf::Image screenshot = window->capture();
				screenshot.saveToFile("screenshots\\" + timestamp + "__" + std::to_string(index) + ".bmp");
				index++;
			}
		}
		simtime.current = clock.getElapsedTime();
        //FIXME Time continue counting in pause mode
	}

	if (((int)(simtime.current.asSeconds())) > (int)(simtime.old.asSeconds()))
	{
		simtime.old = simtime.current;
		fps = frames;
		frames = 0;
		logfile << (int)(simtime.current.asSeconds()) << "; " << world.GetUnitCount() << "; " << dead << std::endl;
	}

	if (settings.screenshot_delay != 0)
	{
		if ((((int)(simtime.current.asSeconds())) % settings.screenshot_delay == 4) && (((int)(simtime.current.asSeconds())) / settings.screenshot_delay == index))
		{
			sf::Image screenshot = window->capture();
			screenshot.saveToFile("screenshots\\" + timestamp + "__" + std::to_string(index) + ".bmp");
			index++;
		}
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
			if (event.key.code == sf::Keyboard::S)
			{
				if (state == st_stop)
					state = st_run;
			}
			if (event.key.code == sf::Keyboard::M)
			{
				drawmode.menu = !drawmode.menu;
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
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				world.units.push_back(Unit(window, &world, &settings, sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y, mousecolor, 0));
			}
		}
		if (event.type == sf::Event::MouseWheelMoved)
		{
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
			{
				menu.ChangeMouseSize(event.mouseWheel.delta * 2);
			}
			else
			{
				hsvmousecolor.h += event.mouseWheel.delta * 5;
				if (hsvmousecolor.h < 0)
				{
					hsvmousecolor.h += 360;
				}
				if (hsvmousecolor.h > 360)
				{
					hsvmousecolor.h = hsvmousecolor.h % 360;
				}
				mousecolor = HsvToRgb(hsvmousecolor);
				menu.SetMouseColor(mousecolor);
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
	if (drawmode.menu)
	{
		menu.Draw();
	}
	window->display();
}

/**************************************************************************************************/

void Simulator::SetTitle()
{
	std::stringstream title;
	title << "Units: " << world.GetUnitCount() << "; Died: " << dead << "; Generation: " << eldest << "-" << youngest << "; Time: " << (int)(simtime.current.asSeconds()) << "s; FPS: " << fps << "; MouseColor: " << ColorToStr(mousecolor);
	window->setTitle(sf::String(title.str()));
}

/**************************************************************************************************/

Simulator::~Simulator()
{
}
