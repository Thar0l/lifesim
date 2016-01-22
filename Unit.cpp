#define _CRT_SECURE_NO_WARNINGS

#include "Unit.h"

sf::Color inv(sf::Color color)
{
	sf::Color result;
	result.r = 255 - color.r;
	result.g = 255 - color.g;
	result.b = 255 - color.b;
	return result;
}

/**************************************************************************************************/

int min(int a, int b)
{
	return (a <= b) ? a : b;
}

/**************************************************************************************************/

int max(int a, int b)
{
	return (a >= b) ? a : b;
}

/**************************************************************************************************/

Unit::Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res)
{
	this->world = world; 
	this->window = window;
	this->settings = settings;
	food  = settings->start_food;
	//TODO replace food and health with one param food
	health = settings->start_health;
	energy = settings->start_energy;
	resist = res;
	speed = settings->speed;
	size = settings->start_size;

	alive = true;
	age = 0;

	image.setPosition(x,y);
	image.setFillColor(resist);
	image.setOrigin(size, size);
	image.setRadius(1 * size);
	image.setOutlineThickness(1);
	image.setOutlineColor(inv(resist));
}

/**************************************************************************************************/

void Unit::Draw()
{
	window->draw(image);
}

/**************************************************************************************************/

void Unit::Move(direction dir)
{
	if ((energy > size) && (dir != stay))
	{
		sf::Vector2f position = image.getPosition();
		int dx;
		int dy;
		direction _dir = dir;

		if (_dir == null)
		{
			int i = rand() % 4;
			switch (i)
			{
			case 0:
				_dir = left;
				break;
			case 1:
				_dir = right;
				break;
			case 2:
				_dir = up;
				break;
			case 3:
				_dir = down;
				break;
			default:
				_dir = left;
				break;
			}
		}
		if (_dir == left)
		{
			position.x -= speed;
		}
		if (_dir == right)
		{
			position.x += speed;
		}
		if (_dir == up)
		{
			position.y -= speed;
		}
		if (_dir == down)
		{
			position.y += speed;
		}
		if (position.x < 0) position.x = 0;
		if (position.y < 0) position.y = 0;
		if (position.x >= world->GetSize().x) position.x = world->GetSize().x - 1;
		if (position.y >= world->GetSize().y) position.y = world->GetSize().y - 1;
		image.setPosition(position);
	}
}

/**************************************************************************************************/

void Unit::Split()
{
	if ((food > settings->con_food_split) && (world->GetUnitCount() < settings->max_units) && (size > 1))
	{
		//std::cout << "(" << (int)(resist.r) << ":" << (int)(resist.g) << ":" << (int)(resist.b) << ") => ";
		int childs = pow(2, size - 1) - 1;
		for (int i = 0; i < childs; i++)
		{
			sf::Color childsresist;
			int r = resist.r + rand() % 11 - 5;
			int g = resist.g + rand() % 11 - 5;
			int b = resist.b + rand() % 11 - 5;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			if (r < 0) r = 0;
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			if ((r < 30) && (g < 30) && (b < 30))
			{
				int x = rand();
				if (x % 3 == 0)
				{
					r = 250;
				}
				else if (x % 3 == 1)
				{
					g = 250;
				}
				else if (x % 3 == 2)
				{
					b = 250;
				}
			}
			childsresist.r = r;
			childsresist.g = g;
			childsresist.b = b;
			world->units.push_back(Unit(window, world, settings, image.getPosition().x + rand()%(4 * size + 1) - 2*size, image.getPosition().y + rand()%(4 * size + 1) - 2*size, childsresist));
			//std::cout << "(" << r << ":" << g << ":" << b << ")";
		}
		//std::cout <<"|"B<< std::endl;
		food = settings->start_food;
		//alive = false;
		size = 1;
	}
}

/**************************************************************************************************/

void Unit::Growth()
{
	if ((food > (settings->con_food_growth) * size ) && (size < 2))
	{
		//food -= (settings->con_food_growth) * size;
		//std::cout << (int)resist.r <<":" << (int)resist.g <<":" << (int)resist.b << std::endl;
		size++;
		image.setRadius(size*1);
		image.setOrigin(size, size);
		image.setFillColor(resist);
	}
}

/**************************************************************************************************/

void Unit::Live()
{
	if (alive)
	{
		age++;
		if (food <= 0)
		{
			//ucount.dead++;
			//FIXME this code should work
			alive = false;
		}
		Growth();
		
		int x = rand() % 1000;
		if (x <= 1)
			Split();

		//int f = food;
		Eat();
		//std::cout << food - f << " : ";
		//f = food;
		Fill();
		//std::cout << food - f << std::endl;
		Move(null);
	}
}

/*************************************************************************************************/

void Unit::Eat()
{
	int tmp = 1;
	int r = size / 2;
	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			if ((i * i + j * j) <= (r*r))
			{
				sf::Color worldpoint = world->GetPoint(image.getPosition().x + i, image.getPosition().y + j);
				if ((worldpoint.r - resist.r) >= 1)
				{
					
					worldpoint.r -= min(tmp, (worldpoint.r - resist.r));
					food += min(tmp, (worldpoint.r - resist.r));
				}
				if ((worldpoint.g - resist.g) >= 1)
				{
					worldpoint.g -= min(tmp, (worldpoint.g - resist.g));
					food += min(tmp, (worldpoint.g - resist.g));
				}
				if ((worldpoint.b - resist.b) >= 1)
				{
					worldpoint.b -= min(tmp, (worldpoint.b - resist.b));
					food += min(tmp, (worldpoint.b - resist.b));
				}
				world->SetPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
			}
		}
	}
}

/**************************************************************************************************/

void Unit::Fill()
{
	int tmp = 12;
	int r = size / 2;
	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			if ((i * i + j * j) <= (r*r))
			{
				sf::Color worldpoint = world->GetPoint(image.getPosition().x + i, image.getPosition().y + j);
				sf::Color old = worldpoint;
				if ((resist.r - worldpoint.r) >= 1)
				{
					food -= min(tmp, (resist.r - worldpoint.r));
					worldpoint.r += min(tmp, (resist.r - worldpoint.r));
				}
				if ((resist.g - worldpoint.g) >= 1)
				{
					food -= min(tmp, (resist.g - worldpoint.g));
					worldpoint.g += min(tmp, (resist.g - worldpoint.g));
				}
				if ((resist.b - worldpoint.b) >= 1)
				{
					food -= (tmp, (resist.b - worldpoint.b));
					worldpoint.b += min(tmp, (resist.b - worldpoint.b));
				}
				world->SetPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
			}
		}
	}
}

/**************************************************************************************************/

int Unit::GetAge()
{
	return age;
}

/**************************************************************************************************/

bool Unit::Alive()
{
	return alive;
}

/**************************************************************************************************/

Unit::~Unit()
{
}