#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "classes.h"


#ifndef __OURCCLASSES_H__
#define __OURCCLASSES_H__


class Base :public Entity 
{
	public:
		Base(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

class Wall :public Entity 
{
	public:
		Wall(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

class Star :public Entity 
{
	public:
		Star(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};
#endif //__OURCCLASSES_H__