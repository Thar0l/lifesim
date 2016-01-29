#define _CRT_SECURE_NO_WARNINGS

#include "Unit.h"

int sign(int x)
{
	return (x == 0) ? 0 : (x > 0) ? 1 : -1;
}


/**************************************************************************************************/

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

Unit::Unit(sf::RenderWindow* window, World* world, Settings* settings, int size, int x, int y, sf::Color res, int generation)
{
	this->world = world; 
	this->window = window;
	this->settings = settings;
	this->food  = settings->food_start;
	this->resist = res;
	this->speed = settings->speed;
	this->size = size;
	this->generation = generation;
	this->alive = true;
	this->age = 0;
	this->id = world->GetNextID();
	this->aim = sf::Vector2f(x, y);

	this->predator = false;

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
	if (predator)
	{
		if (aim == sf::Vector2f(0, 0))
		{
			if (world->HasUnits())
			{
				std::list<Unit>::iterator unit = world->units.begin();
				int min = window->getSize().x;
				while (unit != world->units.end())
				{
					if ((unit->Alive()) && (!unit->Predator()))
					{
						if (unit->GetID() != this->id)
						    if (unit->getDistance(this->image.getPosition()) < min)
							{
								min = unit->getDistance(this->image.getPosition());
								victim = unit;
								aim = victim->GetPosition();
							}
					}
					unit++;
				}
			}
		}
		else if (victim->getDistance(this->image.getPosition()) < size)
		{
			food += victim->BeEaten();
			aim = sf::Vector2f(0, 0);
		}
		else
		{
			aim = victim->GetPosition();
			int	dx = (rand() % 2) * speed * sign(aim.x - image.getPosition().x);
			int	dy = (rand() % 2) * speed * sign(aim.y - image.getPosition().y);
			image.setPosition(sf::Vector2f(image.getPosition().x + dx, image.getPosition().y + dy));
		}
	}
	else
	{
		if (getDistance(aim) < 10)
		{
			aim = sf::Vector2f(image.getPosition().x + rand() % 101 - 50, image.getPosition().y + rand() % 101 - 50);
		}
		else
		{
			int dx = 0;
			int dy = 0;
			if (rand() % 2 == 0)
				dx = (rand() % 3) * speed * sign(aim.x - image.getPosition().x);
			else
				dy = (rand() % 3) * speed * sign(aim.y - image.getPosition().y);
			image.setPosition(sf::Vector2f(image.getPosition().x + dx, image.getPosition().y + dy));
		}
	}
}

/**************************************************************************************************/

void Unit::Split()
{
	if ((food > settings->food_split) && (world->GetUnitCount() < settings->max_units) && (size > 1))
	{
		int childs = 1; // pow(2, size - 1) - 1;
		for (int i = 0; i < childs; i++)
		{
			sf::Color childsresist = resist;
			if ((settings->mutation_diff > 0) && (((double)(rand()) / (double)(RAND_MAX)) <= settings->mutation_chance))
			{
				HsvColor hsv = RgbToHsv(resist);
				int diff = rand() % (2 * settings->mutation_diff + 1) - settings->mutation_diff;
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
			}
			Unit unit = Unit(window, world, settings, settings->size_start, image.getPosition().x + rand() % (4 * size + 1) - 2 * size, image.getPosition().y + rand() % (4 * size + 1) - 2 * size, childsresist, generation + 1);
			if (predator)
			{
				unit.MakePredator();
			}
			world->units.push_back(unit);
		}
		food = settings->food_start;
		if ((settings->mutation_diff > 0) && (((double)(rand()) / (double)(RAND_MAX)) <= settings->mutation_chance))
		{
			HsvColor hsv = RgbToHsv(resist);
			int diff = rand() % (2 * settings->mutation_diff + 1) - settings->mutation_diff;
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
		}
		generation++;
		size = 1;
		if (predator)
		{
			this->MakePredator();
		}
	}
}

/**************************************************************************************************/

void Unit::Growth()
{
	if (((!predator)&&((food > (settings->food_growth) * size ) && (size < settings->max_size))) || ((predator) && ((food >(settings->food_growth) * size) && (size < settings->max_size+2))))
	{
		size++;
		image.setRadius(size);
		image.setOrigin(size, size);
		image.setFillColor(resist);
		this->image.setOutlineColor(inv(resist));
	}
}

/**************************************************************************************************/

int Unit::getDistance(sf::Vector2f target)
{
	sf::Vector2f position = image.getPosition();
	return sqrt((position.x - target.x)*(position.x - target.x) + (position.y - target.y)*(position.y - target.y));
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
		if (((double)(rand()) / (double)(RAND_MAX)) <= settings->split_chance)
		{
			Split();
		}
		if (!predator)
		{
			Eat();
			Fill();
		}
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

long long unsigned int Unit::GetID()
{
	return id;
}

/**************************************************************************************************/

int Unit::BeEaten()
{
	this->alive = false;
	return food;
}

bool Unit::Predator()
{
	return predator;
}

/**************************************************************************************************/

Unit::~Unit()
{
}

sf::Vector2f Unit::GetPosition()
{
	return image.getPosition();
}

void Unit::MakePredator()
{
	this->predator = true;
	this->food += settings->food_start;
	this->size = settings->max_size + 1;
	this->resist = sf::Color(10, 10, 10);
	this->image.setFillColor(resist);
	this->image.setOutlineColor(inv(resist));
	this->image.setRadius(size);
	this->image.setOrigin(size, size);
	this->aim = sf::Vector2f(0, 0);
}