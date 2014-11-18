#define _CRT_SECURE_NO_WARNINGS

#include "Unit.h"

sf::Color neg(sf::Color color)
{
	sf::Color result;
	result.r = 255 - color.r;
	result.g = 255 - color.g;
	result.b = 255 - color.b;
	return result;
}


Unit::Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res)
{
	this->world = world; 
	this->window = window;
	this->settings = settings;
	food = settings->start_food;
	health = settings->start_health;
	energy = settings->start_energy;
	resist = res;
	visiblity = 22;
	speed = settings->speed;
	size = settings->start_size;
	alive = true;
	image.setPosition(x,y);
	image.setFillColor(resist);
	image.setOrigin(size, size);
	image.setRadius(2 * size);
	image.setOutlineThickness(2);
	image.setOutlineColor(neg(resist));
}


void Unit::draw()
{
	window->draw(image);
}


direction Unit::searchFood(int r)
{
	int curfood = world->getPoint(image.getPosition().x, image.getPosition().y).b;
	int maxfood = curfood;
	sf::Vector2f curfoodpoint = sf::Vector2f(image.getPosition().x, image.getPosition().y);
	sf::Vector2f maxfoodpoint = curfoodpoint;

	if (curfood > 30) return null;

	for (int i = -r; i < r; i++)
	{
		for (int j = -r; j < r; j++)
		{
			if (world->getPoint(image.getPosition().x+i, image.getPosition().y+j).b > maxfood)
			{
				maxfood = world->getPoint(image.getPosition().x+i, image.getPosition().y+j).b;
				maxfoodpoint = sf::Vector2f(image.getPosition().x+i, image.getPosition().y+j);
			}
		}
	}

	if (maxfoodpoint != curfoodpoint)
	{
		sf::Vector2f delta = maxfoodpoint - curfoodpoint;
		if (abs(delta.x) > abs(delta.y))
		{
			if (delta.y > 0) return down;
			return up;
		}
		else
		{
			if (delta.x > 0) return right;
			return left;
		}
	}

	return null;
}


void Unit::move(direction dir)
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
		if (position.x >= world->getSize().x) position.x = world->getSize().x - 1;
		if (position.y >= world->getSize().y) position.y = world->getSize().y - 1;
		image.setPosition(position);
	}
}


bool Unit::_eat()
{
	bool eaten = false;
	return eaten;
}


void Unit::split()
{
	if ((food > settings->con_food_split) && (world->getUnitCount() < settings->max_units) && (size > 1))
	{
		sf::Color childsresist[2];
		for (int i = 0; i < 2; i++)
		{
			int r = resist.r + rand() % (21*settings->env_mod) -10 * settings->env_mod;
			int g = resist.g + rand() % (21*settings->env_mod) -10 * settings->env_mod;
			int b = resist.b + rand() % (21*settings->env_mod) -10 * settings->env_mod;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			if (r < 0) r = 0;
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			childsresist[i].r = r;
			childsresist[i].g = g;
			childsresist[i].b = b;
		}
		world->units.push_back(Unit(window, world, settings, image.getPosition().x + rand() % (2 * size + 1) - size, image.getPosition().y + rand() % (4 * size + 1) - size, childsresist[0]));
		world->units.push_back(Unit(window, world, settings, image.getPosition().x + rand() % (2 * size + 1) - size, image.getPosition().y + rand() % (4 * size + 1) - size, childsresist[1]));
		food = 0;
		alive = false;
		size = 0;
	}
	
}


void Unit::growth()
{
	if ((food > (settings->con_food_growth) * size + 60) && (size < 2))
	{
		food -= (settings->con_food_growth) * size;
		size++;
		image.setRadius(size*2);
		image.setOrigin(size, size);
	}
}


void Unit::live()
{
	if (alive)
	{
		if (food <= 0)
		{
			world->died++;
			alive = false;
		}
		growth();
		split();
		eat();
		fill();
		move(null);
	}
}

void Unit::eat()
{
	for (int i = -size; i <= size; i++)
	{
		for (int j = -size; j <= size; j++)
		{
			if ((i * i + j * j) <= (size)* (size))
			{
				
				sf::Color worldpoint = world->getPoint(image.getPosition().x + i, image.getPosition().y + j);
				if ((worldpoint.r - resist.r) >= settings->eat_food)
				{
					if (worldpoint.r >= settings->eat_food)
					worldpoint.r -= settings->eat_food;
					food += settings->eat_food;
				}
				if ((worldpoint.g- resist.g) >= settings->eat_food)
				{
					if (worldpoint.g >= settings->eat_food)
					worldpoint.g -= settings->eat_food;
					food += settings->eat_food;
				}
				if ((worldpoint.b - resist.b) >= settings->eat_food)
				{
					if (worldpoint.b >= settings->eat_food)
					worldpoint.b -= settings->eat_food;
					food += settings->eat_food;
				}
				world->setPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
			}
		}
	}
}

void Unit::fill()
{
	for (int i = -size; i <= size; i++)
	{
		for (int j = -size; j <= size; j++)
		{
			if (((i * i + j * j) <= (size)* (size)))
			{
				sf::Color worldpoint = world->getPoint(image.getPosition().x + i, image.getPosition().y + j);
				sf::Color old = worldpoint;
				if ((resist.r - worldpoint.r) > settings->con_food_spend)
				{
					worldpoint.r += settings->con_food_spend;
					food -= settings->con_food_spend;
				}
				if ((resist.g - worldpoint.g) > settings->con_food_spend)
				{
					worldpoint.g += settings->con_food_spend;
					food -= settings->con_food_spend;
				}
				if ((resist.b - worldpoint.b) > settings->con_food_spend)
				{
					worldpoint.b += settings->con_food_spend;
					food -= settings->con_food_spend;
				}
				world->setPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
			}
		}
	}
}

Unit::~Unit()
{
}


bool Unit::isAlive()
{
	return alive;
}
