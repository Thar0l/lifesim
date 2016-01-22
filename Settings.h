#pragma once
#include "libs.h"
#include <Windows.h>



class Settings
{
public:
	//Application
	int size_x;
	int size_y;
	//Unit
	int start_size;
	int start_food;
	int start_energy;
	int start_health;
	int speed;
	int max_size;
	int eat_food;
	int health_heal;
	int energy_get;
	int health_get;
	int con_energy_move;
	int con_energy_split;
	int con_food_spend;
	int con_food_heal;
	int con_food_energy;
	int con_food_split;
	int con_food_growth;
	int con_health_hunger;
	int env_mod;
	int resist_dif;
	int resist_damage;
	//World
	int start_units;
	int max_units;
	int start_color_r;
	int start_color_g;
	int start_color_b;
	int block_size_x;
	int block_size_y;
	int blur_delay;

	Settings();
	void init(std::string filename);
	void print();
	~Settings();
};

