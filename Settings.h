#pragma once
#include "libs.h"
#include <Windows.h>



class Settings
{
public:
	//Application
	int size_x;
	int size_y;
	int screenshot_delay;
	//Unit
	int size_start;
	int food_start;
	int food_split;
	int food_growth;
	int speed;
	int max_size;
	int food_eat;
	int food_fill;
	int mutation_diff;
	float mutation_chance;
	float split_chance;
	//World
	int start_units;
	int max_units;
	int start_color_r;
	int start_color_g;
	int start_color_b;
	int world_lighthess;

	Settings();
	void init(std::string filename);
	void print();
	~Settings();
};

