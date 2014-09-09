#include "World.h"



bool World::existPoint(int x, int y)
{
	if ((x >= 0) && (y >= 0) && (x < size.x) && (y < size.y)) return true;
	return false;
}


World::World(sf::RenderWindow* window, int width, int height) 
{
	this->window = window;
	settings = new Settings("settings.ini");
	size.x = width;
	size.y = height;
	points.create(size.x, size.y);
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			sf::Color color(settings->start_color_r, settings->start_color_g, settings->start_color_b);
			points.setPixel(i, j, color);
		}
	}
	texture.loadFromImage(points);
	for (int i = 0; i < settings->start_units; i++)
	{
		sf::Color resist;
		resist.b = 0;
		if (i % 5 == 0) 
		{
			resist.r = 240;
			resist.g = 20;
		}
		else if (i % 5 == 1)
		{
			resist.r = 20;
			resist.g = 240;
		}
		else if (i % 5 == 2)
		{
			resist.r = 220;
			resist.g = 220;
		}
		else if (i % 5 == 3)
		{
			resist.r = 120;
			resist.g = 20;
		}
		else
		{
			resist.r = 20;
			resist.g = 120;
		}

		units.push_back(Unit(window, this, rand() % (int)(size.x), rand() % (int)(size.y), resist));
	}
}


void World::draw()
{
	texture.update(points);
	sprite.setTexture(texture);
	window->draw(sprite);
}


sf::Vector2u World::getSize()
{
	return sf::Vector2u(size.x, size.y);
}


int World::getUnitCount()
{
	return units.size();
}


World::~World()
{
}


sf::Color World::getPoint(int x, int y)
{
	if (existPoint(x, y))
	{
		return points.getPixel(x, y);
	}
	return sf::Color::Black;
}


void World::setPoint(int x, int y, sf::Color color)
{
	if (existPoint(x, y))
	{
		points.setPixel(x, y, color);
	}
}


void World::addCircle(int x, int y, int r, sf::Color color)
{
	sf::Color point;
	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			int xcord = x + i;
			int ycord = y + j;
			if (((i * i + j * j) <= (r * r)) && (xcord < size.x) && (xcord >= 0) && (ycord < size.y) && (ycord >= 0))
			{
				point = points.getPixel(xcord, ycord);
				float tmp;
				if (color.r > 0)
				{
					tmp = point.r + color.r - sqrt(i * i + j * j) * color.r / r;
					if (tmp > 255.0) tmp = 255.0;
					point.r = tmp;
				}
				if (color.g > 0)
				{
					tmp = point.g + color.g - sqrt(i * i + j * j) * color.g / r;
					if (tmp > 255.0) tmp = 255.0;
					point.g = tmp;
				}
				if (color.b > 0)
				{
					tmp = point.b + color.b - sqrt(i * i + j * j) * color.b / r;
					if (tmp > 255.0) tmp = 255.0;
					point.b = tmp;
				}
				points.setPixel(xcord, ycord, point);
			}
		}
	}
}


void World::Clear()
{
	{
		for (int i = 0; i < size.x; i++)
		{
			for (int j = 0; j < size.y; j++)
			{
				sf::Color color = sf::Color::Black;
				points.setPixel(i, j, color);
			}
		}
	}
}