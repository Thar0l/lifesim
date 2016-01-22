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

	start_size = reader.GetInteger("Unit", "start_size", 1);
	start_food = reader.GetInteger("Unit", "start_food", 500);
	start_energy = reader.GetInteger("Unit", "start_energy", 30);
	start_health = reader.GetInteger("Unit", "start_health", 100);
	speed = reader.GetInteger("Unit", "speed", 1);
	max_size = reader.GetInteger("Unit", "max_size", 3);
	eat_food = reader.GetInteger("Unit", "eat_food", 10);
	health_heal = reader.GetInteger("Unit", "health_heal", 50);
	energy_get = reader.GetInteger("Unit", "energy_get", 5);
	health_get = reader.GetInteger("Unit", "health_get", 5);
	con_energy_move = reader.GetInteger("Unit", "con_energy_move", 3);
	con_energy_split = reader.GetInteger("Unit", "con_energy_split", 3);
	con_food_spend = reader.GetInteger("Unit", "con_food_spend", 10);
	con_food_heal = reader.GetInteger("Unit", "con_food_heal", 5);
	con_food_energy = reader.GetInteger("Unit", "con_food_energy", 5);
	con_food_split = reader.GetInteger("Unit", "con_food_split",10000);
	con_food_growth = reader.GetInteger("Unit", "con_food_growth", 4000);
	con_health_hunger = reader.GetInteger("Unit", "con_health_hunger", 3);
	env_mod = reader.GetInteger("Unit", "env_mod", 10);
	resist_dif = reader.GetInteger("Unit", "resist_dif", 150);
	resist_damage = reader.GetInteger("Unit", "resist_damage", 1);

	start_units = reader.GetInteger("World", "start_units", 100);
	max_units = reader.GetInteger("World", "max_units", 10000);
	start_color_r = reader.GetInteger("World", "start_color_r", 15);
	start_color_g = reader.GetInteger("World", "start_color_g", 15);
	start_color_b = reader.GetInteger("World", "start_color_b", 15);
	block_size_x = reader.GetInteger("World", "block_size_x", 8);
	block_size_y = reader.GetInteger("World", "block_size_y", 1080);
	blur_delay = reader.GetInteger("World", "blur_delay", 0);
	
	print();
}

void Settings::print()
{
	std::cout << "Settings used: " << std::endl
		<< "size_x : " << size_x << std::endl
		<< "size_y : " << size_y << std::endl
		<< "start_size : " << start_size << std::endl
		<< "start_food : " << start_food << std::endl
		<< "start_energy : " << start_energy << std::endl
		<< "start_health : " << start_health << std::endl
		<< "speed : " << speed << std::endl
		<< "max_size : " << max_size << std::endl
		<< "eat_food : " << eat_food << std::endl
		<< "health_heal : " << health_heal << std::endl
		<< "energy_get : " << energy_get << std::endl
		<< "health_get : " << health_get << std::endl
		<< "con_energy_move : " << con_energy_move << std::endl
		<< "con_energy_split : " << con_energy_split << std::endl
		<< "con_food_spend : " << con_food_spend << std::endl
		<< "con_food_heal : " << con_food_heal << std::endl
		<< "con_food_energy : " << con_food_energy << std::endl
		<< "con_food_split : " << con_food_split << std::endl
		<< "con_food_growth : " << con_food_growth << std::endl
		<< "con_health_hunger : " << con_health_hunger << std::endl
		<< "env_mod : " << env_mod << std::endl
		<< "resist_dif: " << resist_dif << std::endl
		<< "resist_damage: " << resist_damage << std::endl
		<< "start_units : " << start_units << std::endl
		<< "max_units : " << max_units << std::endl
		<< "start_color_r : " << start_color_r << std::endl
		<< "start_color_g : " << start_color_g << std::endl
		<< "start_color_b : " << start_color_b << std::endl
		<< "block_size_x; " << block_size_x << std::endl
		<< "block_size_y; " << block_size_y << std::endl
		<< "blur_delay; " << blur_delay << std::endl;
}


Settings::~Settings()
{
}


