#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Base.h"

using namespace sf;

Base::Base(Image &image, float X, float Y, int W, int H, std::string  Name) :Entity(image, X, Y, W, H, Name)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	life = true;
	sprite.setTextureRect(IntRect(0, 0, 51, 65));
	sprite.setPosition(x, y);
};