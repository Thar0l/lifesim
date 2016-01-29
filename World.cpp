#include "World.h"

World::World()
{}

/**************************************************************************************************/

void World::Init(sf::RenderWindow* window, Settings* settings, int width, int height)
{
	this->window = window;
	this->settings = settings;
	this->size.x = width;
	this->size.y = height;
	this->points.create(size.x, size.y);

	this->Fill(sf::Color(settings->start_color_r, settings->start_color_g, settings->start_color_b));

	texture.loadFromImage(points);
	this->shader.loadFromFile("blur.vert", "blur.frag");
	this->shader.setParameter("texture", sf::Shader::CurrentTexture);
	this->shader.setParameter("width", width);
	this->shader.setParameter("height", height);

	this->AddUnits(settings->start_units);
	
	this->temp = 0;
	this->id = 0;
}

/**************************************************************************************************/

bool World::PointExist(int x, int y)
{
	return ((x >= 0) && (y >= 0) && (x < size.x) && (y < size.y))?true:false;
}

/**************************************************************************************************/


void World::Draw()
{
	sf::RenderStates RenderStates = sf::RenderStates::Default;
	texture.update(points);
	shader.setParameter("texture", texture);
	sprite.setTexture(texture);
	RenderStates.texture = &texture;
	RenderStates.shader = &shader;
	window->draw(sprite, RenderStates);
	//TODO Crete blur() function
	/*BLUR
	temp++;
	if (temp % 100 == 0)
	{
		sf::RenderTexture rendertexture;
		rendertexture.create(size.x, size.y);
		rendertexture.setSmooth(false);
		rendertexture.draw(sprite, &shader);
		points = rendertexture.getTexture().copyToImage();
		texture.update(points);
		sprite.setTexture(texture);
		rendertexture.create(size.x, size.y);
		rendertexture.setSmooth(false);
		rendertexture.draw(sprite, &shader);
		points = rendertexture.getTexture().copyToImage();
	}
	/**/
}

/**************************************************************************************************/

void World::AddUnits(int count)
{
	for (int i = 0; i < count; i++)
	{
		HsvColor resist;
		resist.h = rand() % 360;
		resist.s = 255;
		resist.v = 255;
		units.push_back(Unit(window, this, settings, settings->size_start, rand() % (int)(size.x), rand() % (int)(size.y), HsvToRgb(resist)));

	}
}

/**************************************************************************************************/

sf::Vector2u World::GetSize()
{
	return sf::Vector2u(size.x, size.y);
}

/**************************************************************************************************/

int World::GetUnitCount()
{
	return units.size();
}

/**************************************************************************************************/

sf::Color World::GetPoint(int x, int y)
{
	return (PointExist(x, y))?(points.getPixel(x, y)):(sf::Color::Black);
}

/**************************************************************************************************/

void World::SetPoint(int x, int y, sf::Color color)
{
	if (PointExist(x, y))
	{
		points.setPixel(x, y, color);
	}
}

/**************************************************************************************************/

void World::AddCircle(int x, int y, int r, sf::Color color)
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
					tmp = color.r + point.r - sqrt(i * i + j * j) * color.r / r;
					if (tmp > 255.0) tmp = 255.0;
					point.r = tmp;
				}
				if (color.g > 0)
				{
					tmp = color.g + point.g - sqrt(i * i + j * j) * color.g / r;
					if (tmp > 255.0) tmp = 255.0;
					point.g = tmp;
				}
				if (color.b > 0)
				{
					tmp = color.b + point.b - sqrt(i * i + j * j) * color.b / r;
					if (tmp > 255.0) tmp = 255.0;
					point.b = tmp;
				}
				points.setPixel(xcord, ycord, point);
			}
		}
	}
}

/**************************************************************************************************/

void World::Fill(sf::Color color)
{
    for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			points.setPixel(i, j, color);
		}
	}
}

/**************************************************************************************************/

void World::FillCircles()
{
	for (int i = 0; i < (rand() % (10 * settings->world_lighthess) + settings->world_lighthess); i++)
	{
		int t = i;
		int f = 255;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		AddCircle(rand() % size.x, rand() % size.y, rand() % 200 + 80, sf::Color(r, g, b));
	}
	for (int i = 0; i < (rand() % (20 *settings->world_lighthess) + 2 * settings->world_lighthess); i++)
	{
		int t = rand() % 3;
		int f = 255;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		AddCircle(rand() % size.x, rand() % size.y, rand() % 100 + 120, sf::Color(r, g, b));
	}
	for (int i = 0; i < (rand() % (8 * settings->world_lighthess) + settings->world_lighthess / 2); i++)
	{
		int t = rand() % 3;
		int f = rand() % 160;
		int r = 0;
		int g = 0;
		int b = 0;
		if (t % 3 == 0) r = f;
		if (t % 3 == 1) g = f;
		if (t % 3 == 2) b = f;
		AddCircle(rand() % size.x, rand() % size.y, rand() % 320 + 220, sf::Color(r, g, b));
	}
}

/**************************************************************************************************/

bool World::HasUnits()
{
	return (units.size() > 0)?true:false;
}

/**************************************************************************************************/

void World::Clear()
{
	this->Fill(sf::Color::Black);
}

/**************************************************************************************************/

long long unsigned int World::GetNextID()
{
	return id++;
}

/**************************************************************************************************/

World::~World()
{
}