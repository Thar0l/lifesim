#pragma once

struct point_float
{
	float x;
	float y;
};

struct point_int
{
	int x;
	int y;
};

enum direction 
{
	null = 0,
	left = 1,
	right = 2,
	up = 3, 
	down = 4,
	stay = 5
};