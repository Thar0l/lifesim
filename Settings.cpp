#include "Settings.h"

Settings::Settings()
{
}

void Settings::init(std::string filename)
{
	INIReader reader("settings.ini");
	if (reader.ParseError() < 0) 
	{
		std::cout << "Can't load " << filename << std::endl;
	}
	else
	{
		std::cout << "Settings loaded from " << filename << std::endl;
	}


	size_x = reader.GetInteger("App", "size_x", 1904);
	size_y = reader.GetInteger("App", "size_y", 1000);
	screenshot_delay = reader.GetInteger("App", "screenshot_delay", 60);
	size_start = reader.GetInteger("Unit", "size_start", 1);
	food_start = reader.GetInteger("Unit", "food_start", 200);
	speed = reader.GetInteger("Unit", "speed", 1);
	max_size = reader.GetInteger("Unit", "max_size", 3);
	food_eat = reader.GetInteger("Unit", "food_eat", 4);
	food_fill = reader.GetInteger("Unit", "food_fill", 4);
	food_split = reader.GetInteger("Unit", "food_split",4000);
	food_growth = reader.GetInteger("Unit", "food_growth", 3000);
	mutation_diff = reader.GetInteger("Unit", "mutation_diff", 5);
	mutation_chance = reader.GetReal("Unit", "mutation_chance", 0.01);
	split_chance = reader.GetReal("Unit", "split_chance", 0.5);
    start_units = reader.GetInteger("World", "start_units", 100);
	max_units = reader.GetInteger("World", "max_units", 10000);
	start_color_r = reader.GetInteger("World", "start_color_r", 15);
	start_color_g = reader.GetInteger("World", "start_color_g", 15);
	start_color_b = reader.GetInteger("World", "start_color_b", 15);
	world_lighthess = reader.GetInteger("World", "world_lighthess", 5);
	print();
}

void Settings::print()
{
	std::cout << "Settings used: " << std::endl
		<< "size_x : " << size_x << std::endl
		<< "size_y : " << size_y << std::endl
		<< "screenshot_delay : " << screenshot_delay << std::endl
		<< "size_start : " << size_start << std::endl
		<< "food_start : " << food_start << std::endl
		<< "speed : " << speed << std::endl
		<< "max_size : " << max_size << std::endl
		<< "food_eat : " << food_eat << std::endl
		<< "food_fill : " << food_fill << std::endl
		<< "food_split : " << food_split << std::endl
		<< "food_growth : " << food_growth << std::endl
		<< "mutation_diff : " << mutation_diff << std::endl
		<< "mutation_chance : " << mutation_chance << std::endl
		<< "split_chance : " << split_chance << std::endl
		<< "start_units : " << start_units << std::endl
		<< "max_units : " << max_units << std::endl
		<< "start_color_r : " << start_color_r << std::endl
		<< "start_color_g : " << start_color_g << std::endl
		<< "start_color_b : " << start_color_b << std::endl
		<< "world_lighthess: " << world_lighthess << std::endl;
}


Settings::~Settings()
{
}


