#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __BASE_H__
#define __BASE_H__

class Base :public Entity 
{
	public:
		Base(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

#endif //__BASE_H__