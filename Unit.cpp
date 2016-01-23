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

HsvColor RgbToHsv(sf::Color rgb)
{
	HsvColor hsv;
	float min, max, delta;
	float r = rgb.r / 255.0;
	float g = rgb.g / 255.0;
	float b = rgb.b / 255.0;
	float h = 0, s, v;

	min = std::min(r, g);
	min = std::min(min, b);

	max = std::max(r, g);
	max = std::max(max, b);

	v = max;				// v
	delta = max - min;
	if (max != 0)
		s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = 0;
		hsv.h = h;
		hsv.s = 255 * s;
		hsv.v = 255 * v;
		return hsv;
	}
	if (r == max)
		h = (g - b) / delta;		// between yellow & magenta
	else if (g == max)
		h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		h = 4 + (r - g) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	if (h < 0)
		h += 360;
	if (delta == 0.0) h = 0;
	hsv.h = h;
	hsv.s = 255 * s;
	hsv.v = 255 * v;
	return hsv;
}

/**************************************************************************************************/

sf::Color HsvToRgb(HsvColor hsv)
{
	sf::Color rgb;
	int i;
	float f, p, q, t;
	float h = hsv.h;
	float s = hsv.s / 255.0;
	float v = hsv.v / 255.0;
	float r, g, b;
	if (s == 0) {
		// achromatic (grey)
		r = g = b = v;
		rgb.r = 255 * r;
		rgb.g = 255 * g;
		rgb.b = 255 * b;
		return rgb;
	}
	h /= 60;			// sector 0 to 5
	i = (int)floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:		// case 5:
		r = v;
		g = p;
		b = q;
		break;
	}
	rgb.r = 255 * r;
	rgb.g = 255 * g;
	rgb.b = 255 * b;
	return rgb;
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

Unit::Unit(sf::RenderWindow* window, World* world, Settings* settings, int x, int y, sf::Color res, int generation)
{
	this->world = world; 
	this->window = window;
	this->settings = settings;
	this->food  = settings->food_start;
	this->resist = res;
	this->speed = settings->speed;
	this->size = settings->size_start;
	this->generation = generation;
	this->alive = true;
	this->age = 0;

	this->image.setPosition(x,y);
	this->image.setFillColor(resist);
	this->image.setOrigin(size, size);
	this->image.setRadius(size);
	this->image.setOutlineThickness(1);
	this->image.setOutlineColor(inv(resist));
}

/**************************************************************************************************/

void Unit::Draw()
{
	window->draw(image);
}

/**************************************************************************************************/

void Unit::Move(direction dir)
{
	if (dir != stay)
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
	if ((food > settings->food_split) && (world->GetUnitCount() < settings->max_units) && (size > 1))
	{
		int childs = pow(2, size - 1) - 1;
		for (int i = 0; i < childs; i++)
		{
			sf::Color childsresist;
			HsvColor hsv = RgbToHsv(resist);
			int diff = rand() % (2*settings->mutation_diff + 1) - settings->mutation_diff;
			hsv.h += diff;
			if (hsv.h < 0)
			{
				hsv.h += 360;
			}
			if (hsv.h > 360)
			{
				hsv.h = hsv.h % 360;
			}
			childsresist = HsvToRgb(hsv);
			world->units.push_back(Unit(window, world, settings, image.getPosition().x + rand()%(4 * size + 1) - 2*size, image.getPosition().y + rand()%(4 * size + 1) - 2*size, childsresist, generation + 1));
		}
		food = settings->food_start;
		HsvColor hsv = RgbToHsv(resist);
		int diff = rand() % 21 - 10;
		hsv.h += diff;
		if (hsv.h < 0)
		{
			hsv.h += 360;
		}
		if (hsv.h > 360)
		{
			hsv.h = hsv.h % 360;
		}
		resist = HsvToRgb(hsv);
		image.setFillColor(resist);
		image.setOutlineColor(inv(resist));
		generation++;
		size = 1;
	}
}

/**************************************************************************************************/

void Unit::Growth()
{
	if ((food > (settings->food_growth) * size ) && (size < settings->max_size))
	{
		size++;
		image.setRadius(size);
		image.setOrigin(size, size);
		image.setFillColor(resist);
		this->image.setOutlineColor(inv(resist));
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
			alive = false;
		}
		Growth();
		
		int x = rand() % 1000;
		if (x <= 1)
			Split();
		Eat();
		Fill();
		Move(null);
	}
}

/*************************************************************************************************/

void Unit::Eat()
{
	int eat = round(settings->food_eat * (resist.r + resist.g + resist.b) / 255.0);
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
					
					worldpoint.r -= min(eat, (worldpoint.r - resist.r));
					food += min(eat, (worldpoint.r - resist.r));
				}
				if ((worldpoint.g - resist.g) >= 1)
				{
					worldpoint.g -= min(eat, (worldpoint.g - resist.g));
					food += min(eat, (worldpoint.g - resist.g));
				}
				if ((worldpoint.b - resist.b) >= 1)
				{
					worldpoint.b -= min(eat, (worldpoint.b - resist.b));
					food += min(eat, (worldpoint.b - resist.b));
				}
				world->SetPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
			}
		}
	}
}

/**************************************************************************************************/

void Unit::Fill()
{
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
					food -= min(settings->food_fill, (resist.r - worldpoint.r));
					worldpoint.r += min(settings->food_fill, (resist.r - worldpoint.r));
				}
				if ((resist.g - worldpoint.g) >= 1)
				{
					food -= min(settings->food_fill, (resist.g - worldpoint.g));
					worldpoint.g += min(settings->food_fill, (resist.g - worldpoint.g));
				}
				if ((resist.b - worldpoint.b) >= 1)
				{
					food -= min(settings->food_fill, (resist.b - worldpoint.b));
					worldpoint.b += min(settings->food_fill, (resist.b - worldpoint.b));
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

int Unit::GetGeneration()
{
	return this->generation;
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