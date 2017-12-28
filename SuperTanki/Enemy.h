#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __ENEMY_H__
#define __ENEMY_H__

class Enemy :public Entity{
public:
	int direction;//направление движения врага
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name);
	void checkCollisionWithMap(float Dx, float Dy);//ф-ция проверки столкновений с картой
	void update(float time);
};

#endif //__ENEMY_H__