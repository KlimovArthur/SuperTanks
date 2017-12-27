#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"

#ifndef __CCLASSES_H__
#define __CCLASSES_H__


using namespace sf;

class Entity {
public:
	enum { left, right, up, down, stay} state;// тип перечислени€ - состо€ние объекта
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер дл€ будущих целей
	int w, h; //переменна€ УhealthФ, хран€ща€ жизни игрока
	bool life; //переменна€ УlifeФ жизнь, логическа€
	bool check;
	int direction;
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт 
	float CurrentFrame;//хранит текущий кадр
	std::string name;//враги могут быть разные, врагов можно различать по именам
	//каждому можно дать свое действие в update() в зависимости от имени

	Entity(Image &image, float X, float Y, int W, int H, std::string Name);

	FloatRect getRect();
	
	virtual void update(float time) = 0;

};

class Player :public Entity 
{
	public:
		Player(Image &image, float X, float Y, int W, int H, std::string Name);
		void control();
		//ћетод проверки столкновений с элементами карты
		void checkCollisionWithMap(float Dx, float Dy);

		void update(float time);
};

class Enemy :public Entity{
public:
	int direction;//направление движени€ врага
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name);
	void checkCollisionWithMap(float Dx, float Dy);//ф-ци€ проверки столкновений с картой
	void update(float time);
};

class Bullet :public Entity{//класс пули
public:
	int direction;//направление пули
	//всЄ так же, только вз€ли в конце состо€ние игрока (int dir) 
	//дл€ задани€ направлени€ полЄта пули
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2);
	void update(float time);
};

#endif //__CCLASSES_H__