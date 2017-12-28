#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Star.h"

using namespace sf;

Star::Star(Image &image, float X, float Y, int W, int H, std::string  Name) :Entity(image, X, Y, W, H, Name)
{
		x = X;
		y = Y;
		w = W;
		h = H;
		life = true;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);

};