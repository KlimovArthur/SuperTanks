#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Wall.h"

using namespace sf;

Wall::Wall(Image &image, float X, float Y, int W, int H, std::string  Name) :Entity(image, X, Y, W, H, Name)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	life = true;
	sprite.setTextureRect(IntRect(160, 0, w, h));
	sprite.setPosition(x, y);
};