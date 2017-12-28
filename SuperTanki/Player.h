#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player :public Entity 
{
	public:
		Player(Image &image, float X, float Y, int W, int H, std::string Name);
		void control();
		//Метод проверки столкновений с элементами карты
		void checkCollisionWithMap(float Dx, float Dy);

		void update(float time);
};
#endif //__PLAYER_H__