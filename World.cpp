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
			sf::Color color;
			color.r = settings->start_color_r;
			color.g = settings->start_color_g;
			color.b = settings->start_color_b;
			points.setPixel(i, j, color);
		}
	}

	shader.loadFromFile("blur.vert", "blur.frag");
	shader.setParameter("texture", sf::Shader::CurrentTexture);
	shader.setParameter("width", this->size.x);
	shader.setParameter("height", this->size.y);
	texture.loadFromImage(points);
	render_texture.setSmooth(false);
	tmp = 0;
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
	/*
	sf::Shader Blur;
	Blur.loadFromFile("blur.frag",sf::Shader::Type::Fragment);
	
	sf::Texture texture;
	texture.loadFromImage(points);
	sf::Sprite sprite;
	sprite.setTexture(texture, true);
	Blur.setParameter("texture", texture);
	Blur.setParameter("blur_radius", 5);
	sf::RenderStates RenderStates = sf::RenderStates::Default;
	RenderStates.shader = &Blur;
	//RenderStates.texture = &texture;
	window->draw(sprite, RenderStates);*/


	texture.update(points);
	sprite.setTexture(texture);
	tmp++;
	window->draw(sprite);
	/*if (tmp == 500)
	{
		points = texture.copyToImage();
		tmp = 0;
	}*/

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






void World::addCircle(int x, int y, int r, int cr, int cg, int cb)
{
	sf::Color color;
	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			int xcord = x + i;
			int ycord = y + j;
			if (((i * i + j * j) <= (r * r)) && (xcord < size.x) && (xcord >= 0) && (ycord < size.y) && (ycord >= 0))
			{
				color = points.getPixel(xcord, ycord);
				float tmp;
				if (cr > 0)
				{
					tmp = color.r + cr - sqrt(i * i + j * j) * cr / r;
					if (tmp > 255.0) tmp = 255.0;
					color.r = tmp;
				}
				if (cg > 0)
				{
					tmp = color.g + cg - sqrt(i * i + j * j) * cg / r;
					if (tmp > 255.0) tmp = 255.0;
					color.g = tmp;
				}
				if (cb > 0)
				{
					tmp =  color.b + cb - sqrt(i * i + j * j) * cb / r;
					if (tmp > 255.0) tmp = 255.0;
					color.b = tmp;
				}
				points.setPixel(xcord, ycord, color);
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
				sf::Color color;
				color.r = 0;
				color.g = 0;
				color.b = 0;
				points.setPixel(i, j, color);
			}
		}
	}
}

void World::BlurPoint(int i, int j)
{
	if ((i > 0) && (j > 0) && (i < size.x - 1) && (j < size.y - 1))
	{
		sf::Color color = points.getPixel(i, j);
		color.r = ( 
			points.getPixel(i - 1, j - 1).r + points.getPixel(i - 1, j).r + points.getPixel(i - 1, j + 1).r +
			points.getPixel(i, j - 1).r + points.getPixel(i, j).r + points.getPixel(i, j + 1).r +
			points.getPixel(i + 1, j - 1).r + points.getPixel(i + 1, j).r + points.getPixel(i + 1, j + 1).r 
			) / 9;
		color.g = (
			points.getPixel(i - 1, j - 1).g + points.getPixel(i - 1, j).g + points.getPixel(i - 1, j + 1).g +
			points.getPixel(i, j - 1).g + points.getPixel(i, j).g + points.getPixel(i, j + 1).g +
			points.getPixel(i + 1, j - 1).g + points.getPixel(i + 1, j).g + points.getPixel(i + 1, j + 1).g
			) / 9;
		color.b = (
			points.getPixel(i - 1, j - 1).b + points.getPixel(i - 1, j).b + points.getPixel(i - 1, j + 1).b +
			points.getPixel(i, j - 1).b + points.getPixel(i, j).b + points.getPixel(i, j + 1).b +
			points.getPixel(i + 1, j - 1).b + points.getPixel(i + 1, j).b + points.getPixel(i + 1, j + 1).b
			) / 9;
		points.setPixel(i, j, color);
	}
}

void World::Blur(int i)
{
	int startx = 1;
	int endx = size.x;
	int starty = 1;
	int endy = size.y;
	if (i == 1)
	{
		startx = 1;
		starty = 1;
		endx = size.x / 2;
		endy = size.y / 2;
	}
	if (i == 2)
	{
		startx = 1;
		starty = size.y / 2;
		endx = size.x / 2;
		endy = size.y - 1;
	}
	if (i == 3)
	{
		startx = size.x / 2;
		starty = 1;
		endx = size.x - 1;
		endy = size.y / 2;
	}
	if (i == 4)
	{
		startx = size.x / 2;
		starty = size.y / 2;
		endx = size.x - 1;
		endy = size.y - 1;
	}
	for (int i = 1; i < (size.x-1); i++)
	{
		for (int j = 1; j < (size.y-1); j++)
		{
			BlurPoint(i, j);
		}
	}
}

void World::BlurBlock(int x, int y)
{
	
	int countx = size.x / settings->block_size_x + 1;
	int county = size.y / settings->block_size_y + 1;
	if ((x < countx) && (y < county))
	{
		int startx = x*settings->block_size_x;
		int starty = y*settings->block_size_y;
		if (startx >= size.x - 1) return;
		if (starty >= size.y - 1) return;
		if (startx == 0) startx = 1;
		if (starty == 0) starty = 1;
		int endx = (x + 1)*settings->block_size_x;
		int endy = (y + 1)*settings->block_size_y;
		if (endx >= size.x - 1) endx = size.x;
		if (endy >= size.y - 1) endy = size.y;
		for (int i = startx; i < endx; i++)
		{
			for (int j = starty; j < endy; j++)
			{
				BlurPoint(i, j);
			}
		}
	}
}