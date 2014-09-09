#define _CRT_SECURE_NO_WARNINGS

#include "Unit.h"


Unit::Unit(sf::RenderWindow* window, World* world, int x, int y, sf::Color res)
{
	this->world = world; 
	this->window = window;
	food = world->settings->start_food;
	health = world->settings->start_health;
	energy = world->settings->start_energy;
	resist = res;
	visiblity = 22;
	speed = world->settings->speed;
	size = world->settings->start_size;
	alive = true;
	image.setPosition(x,y);
	image.setFillColor(resist);
	image.setOrigin(size, size);
	image.setRadius(2 * size);
	image.setOutlineThickness(2);
	image.setOutlineColor(sf::Color::Blue);
	/*if ((resist.r < 20) && (resist.g < 20))
		image.setOutlineColor(sf::Color::Black);
	else if (resist.r > resist.g) 
		image.setOutlineColor(sf::Color::Magenta); 
	else if (resist.r < resist.g)
		image.setOutlineColor(sf::Color::Green);
	else
		image.setOutlineColor(sf::Color::Yellow);*/
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

	if (curfood > 0) return null;

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
		if (abs(delta.x) < abs(delta.y))
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
		energy -= (world->settings->con_energy_move)*size;
		image.setPosition(position);
	}
}

bool Unit::eat()
{
	bool eaten = false;
	int t = 0;
	int i = 0;
	int j = 0;
//	std::cout << "!";
	for (int i = -size; i <= size; i++)
	{
		for (int j = -size; j <= size; j++)
		{
			if ((i * i + j * j) <= (size) * (size)) 
			{
				sf::Color worldpoint = world->getPoint(image.getPosition().x + i, image.getPosition().y + j);
				if (worldpoint.b >= world->settings->eat_food)
				{
//					std::cout << "*";
					worldpoint.b -= world->settings->eat_food;
					food += world->settings->eat_food;
					t += 1;
					world->setPoint(image.getPosition().x + i, image.getPosition().y + j, worldpoint);
					move(null);
					eaten = true;
				}
			}
		}
	}
//	std::cout << "eaten: "<<t<<std::endl;
	return eaten;
}



void Unit::split()
{
	
	if ((food > world->settings->con_food_split) && (world->getUnitCount() < world->settings->max_units) && (size > 1))
	{
		sf::Color childsresist[2];
		for (int i = 0; i < 2; i++)
		{
			int r = resist.r + rand() % 21 - 10;
			int g = resist.g + rand() % 21 - 10;
			int b = resist.b + rand() % 21 - 10;
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
		std::cout << (int)(resist.r) << ":" << (int)(resist.g) << ":" << (int)(resist.b);
		std::cout << " >> ";
		std::cout << (int)(childsresist[0].r) << ":" << (int)(childsresist[0].g) << ":" << (int)(childsresist[0].b);
		std::cout << " + ";
		std::cout << (int)(childsresist[1].r) << ":" << (int)(childsresist[1].g) << ":" << (int)(childsresist[1].b) << std::endl;
		world->units.push_back(Unit(window, world, image.getPosition().x + rand() % (2 * size + 1) - size, image.getPosition().y + rand() % (2 * size + 1) - size, childsresist[0]));
		world->units.push_back(Unit(window, world, image.getPosition().x + rand() % (2 * size + 1) - size, image.getPosition().y + rand() % (2 * size + 1) - size, childsresist[1]));
		food = 0;
		alive = false;
		size = 0;
	}
	
}

void Unit::growth()
{
	if ((food > (world->settings->con_food_growth) * size + 60) && (size < 2))
	{
		food -= (world->settings->con_food_growth) * size;
		size++;
		image.setRadius(size*2);
		image.setOrigin(size, size);
	}
}

void Unit::live()
{
	if (alive)
	{
		food -= (world->settings->con_food_spend)*(size*size);
		for (int i = -size; i <= size; i++)
		{
			for (int j = -size; j <= size; j++)
			{
				if ((i * i + j * j) <= (size)* (size))
				{
					sf::Color env = world->getPoint(image.getPosition().x+i, image.getPosition().y+j);
					int dif = sqrt((env.r - resist.r)*(env.r - resist.r) + (env.g - resist.g)*(env.g - resist.g));
					if (dif != 0)
					{
						//std::cout << dif << std::endl;
						int r = env.r;
						int g = env.g;
						int b = env.b;
						if (dif >= (world->settings->resist_dif)) health -= (world->settings->resist_damage);
						if (r > resist.r) r -= (world->settings->env_mod);
						if (r < resist.r) r += (world->settings->env_mod);
						if (g > resist.g) g -= (world->settings->env_mod);
						if (g < resist.g) g += (world->settings->env_mod);
						if (r > 255) r = 255;
						if (g > 255) g = 255;
						if (b > 255) b = 255;
						if (r < 0) r = 0;
						if (g < 0) g = 0;
						if (b < 0) b = 0;
						env.r = r;
						env.g = g;
						env.b = b;
						world->setPoint(image.getPosition().x + i, image.getPosition().y + j, env);
					}

				}
				/*if ( ((image.getPosition().x + i) > 0) && ((image.getPosition().y + j) > 0) && ((image.getPosition().x + i) < (world->getSize().x - 1)) && ((image.getPosition().y + j) < (world->getSize().y) - 1))
					world->BlurPoint(image.getPosition().x + i, image.getPosition().y + j);*/
			}
		}
		if ((health < (world->settings->health_heal)) && (food > (world->settings->con_food_heal)))
		{
			food -= (world->settings->con_food_heal);
			health += (world->settings->health_get);
		}
		if (health > 100) health = 100;
		if ((energy < (world->settings->con_energy_move)) && (food >(world->settings->con_food_energy)))
		{
			food -= (world->settings->con_food_energy);
			energy += (world->settings->energy_get);
		}
		if (food <= 0)
		{
			health -= (world->settings->con_health_hunger);
		}
		if (health <= 0)
		{
			alive = false;
			world->addCircle(image.getPosition().x, image.getPosition().y, size*2, 0, 0, food/(size*size));
			size = 0;
		}
		//std::cout << "H: " << health << ". F: " << food << ". E: " << energy << std::endl;
		growth();
		int tmp = rand() % 100;
		if (tmp < 10)
			split();
		if (!eat())
		{
			
			direction dir = searchFood(size*3);
			/*
			for (int i = -size; i <= size; i++)
			{
				for (int j = -size; j <= size; j++)
				{
					if ((image.getPosition().x + i > 0) && (image.getPosition().y + j > 0) && (image.getPosition().x + i < world->getSize().x - 1) && (image.getPosition().y + j < world->getSize().y) - 1)
						world->BlurPoint(image.getPosition().x + i, image.getPosition().y + j);
				}
			}
			*/
			move(dir);
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