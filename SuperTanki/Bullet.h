#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Entity.h"

#ifndef __BULLET_H__
#define __BULLET_H__

class Bullet :public Entity{//класс пули
public:
	int direction;//направление пули
	//всё так же, только взяли в конце состояние игрока (int dir) 
	//для задания направления полёта пули
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2);
	void update(float time);
};


#endif //__BULLET_H__