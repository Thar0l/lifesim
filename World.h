#pragma once

#include "libs.h"
#include "Unit.h"

class World
{
private:
	sf::RenderWindow* window;
	Settings* settings;

	sf::Vector2u size;
	sf::Image points;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Shader shader;
	//TODO delete temp
	int temp;

	bool PointExist(int x, int y);
	void AddCircle(int x, int y, int r, sf::Color color);

public:
	
	std::list<Unit> units;

	World();
	void Init(sf::RenderWindow* window, Settings* settings, int width, int height);
	sf::Color GetPoint(int x, int y);
	int GetUnitCount();
	sf::Vector2u GetSize();
	void SetPoint(int x, int y, sf::Color color);
	void AddUnits(int count);
	void Fill(sf::Color color);
	void FillCircles();
	void Draw();
	//TODO Add blur() function
	//void Blur();
	bool HasUnits();
	void Clear();
	~World();
	
	
	
	
	
};