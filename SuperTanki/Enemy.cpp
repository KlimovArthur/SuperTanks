#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Enemy.h"


using namespace sf;

Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	if (name == "EasyEnemy")
	{
		//«адаем спрайту один пр€моугольник дл€
		//вывода одного игрока. IntRect Ц дл€ приведени€ типов
		sprite.setTextureRect(IntRect(0, 0, w, h));
		direction = rand() % (3); //Ќаправление движени€ врага задаЄм случайным образом
		//через генератор случайных чисел
		speed = 0.1;//даем скорость.этот объект всегда двигаетс€
		dx = speed;
		dy = speed;
	}
}

void Enemy::checkCollisionWithMap(float Dx, float Dy)//ф-ци€ проверки столкновений с картой
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимс€ по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'e'))//если элемент - тайлик стены
			{
				if (Dy > 0) 
				{
					y = i * 32 - h;  dy = -0.1; 
					direction = rand() % (4); //Ќаправление движени€ врага
				}//по Y 
				if (Dy < 0)
				{
					y = i * 32 + 32; dy = 0.1; 
					direction = rand() % (4);//Ќаправление движени€ врага 
				}//столкновение с верхними кра€ми 
				if (Dx > 0)
				{
					x = j * 32 - w; dx = -0.1; 
					direction = rand() % (4);//Ќаправление движени€ врага 
				}//с правым краем карты
				if (Dx < 0)
				{
					x = j * 32 + 32; dx = 0.1; 
					direction = rand() % (4); //Ќаправление движени€ врага
				}// с левым краем карты
			}
		}
	}

void Enemy::update(float time)
	{
		if (name == "EasyEnemy")
		{//дл€ персонажа с таким именем логика будет такой
			if (life) 
			{//провер€ем, жив ли герой
				switch (direction)//делаютс€ различные действи€ в зависимости от состо€ни€
				{
					case 0:
					{//состо€ние идти вправо
						dx = speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 77, 77, 80));
						state = right;
						break;
					}
					case 1:
					{//состо€ние идти влево
						dx = -speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 158, 77, 80));
						state = left;
						break;
					}
					case 2:
					{//идти вверх
						dy = -speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 77, 80));
						state = up;
						break;
					}
					case 3:
					{//идти вниз
						dy = speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 240, 77, 80));
						state = down;
					break;
					}
				}
				x += dx*time; //движение по УXФ
				checkCollisionWithMap(dx, 0);//обрабатываем столкновение по ’
				y += dy*time; //движение по УYФ
				checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

				sprite.setPosition(x, y); //спрайт в позиции (x, y).

			}
		}
	}