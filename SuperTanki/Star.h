#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __STAR_H__
#define __STAR_H__

class Star :public Entity 
{
	public:
		Star(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

#endif //__STAR_H__