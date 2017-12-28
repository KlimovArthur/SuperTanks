#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __WALL_H__
#define __WALL_H__

class Wall :public Entity 
{
	public:
		Wall(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

#endif //__WALL_H__