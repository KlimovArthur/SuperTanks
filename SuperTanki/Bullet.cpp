#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Bullet.h"


using namespace sf;

Bullet::Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2) :Entity(image, X, Y, W, H, Name)
{
	x = X+20;
	y = Y+5;
	direction = dir;
	speed = 0.8;
	w = h = 16;
	life = true;
	check = check2;
		//���� ������������� � ������������
};

void Bullet::update(float time)
	{
		switch (direction)
		{
			case 0: dx = -speed; dy = 0;   break;// state = left
			case 1: dx = speed; dy = 0;   break;// state = right
			case 2: dx = 0; dy = -speed;   break;// state = up
			case 3: dx = 0; dy = speed;   break;// state = down
		}

		if (life)
		{
			x += dx*time;//���� �������� ���� �� �
			y += dy*time;//�� �

		if (x <= 0) x = 20;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������ (������ ����� ���������!)
		if (y <= 0) y = 20;

		if (x >= 1280) x = 1260;// �������� ���� � ������ �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������ (������ ����� ���������!)
		if (y >= 704) y = 684;


			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, ��
						life = false;// �� ���� �������
				}
			sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
		}
	}

			
	