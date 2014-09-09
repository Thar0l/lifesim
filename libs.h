#pragma once

#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <math.h>

#include "general.h"
#include "INIReader.h"
#include "Settings.h"

class World;
class Unit;