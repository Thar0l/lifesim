#include "World.h"



bool World::existPoint(int x, int y)
{
	if ((x >= 0) && (y >= 0) && (x < size.x) && (y < size.y)) return true;
	return false;
}


World::World(sf::RenderWindow* window, Settings* settings, int width, int height)
{
	this->window = window;
	this->settings = settings;
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
		if (i % 3 == 0) 
		{
			resist.r = 250;
			resist.g = 0;
			resist.b = 0;
		}
		else if (i % 3 == 1)
		{
			resist.r = 0;
			resist.g = 250;
			resist.b = 0;
		}
		else if (i % 3 == 2)
		{
			resist.r = 0;
			resist.g = 0;
			resist.b = 250;
		}/*
		else if (i % 6 == 3)
		{
			resist.r = 240;
			resist.g = 240;
			resist.b = 0;
		}
		else if (i % 6 == 4)
		{
			resist.r = 240;
			resist.g = 0;
			resist.b = 240;
		}
		else
		{
			resist.r = 0;
			resist.g = 240;
			resist.b = 240;
		}*/

		units.push_back(Unit(window, this, settings, rand() % (int)(size.x), rand() % (int)(size.y), resist));
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
				sf::Color color = sf::Color(250, 125, 125);
				points.setPixel(i, j, color);
			}
		}
	}
}