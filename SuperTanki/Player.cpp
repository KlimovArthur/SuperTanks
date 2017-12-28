#include "Entity.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Player.h"


using namespace sf;

Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	state = stay;
	if (name == "Player1")
	{
		//������ ������� ���� ������������� ���
		//������ ������ ������. IntRect � ��� ���������� �����
		sprite.setTextureRect(IntRect(0, 0, 77, 80));
	}
}

void Player::control()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
			state = left;
			speed = 0.1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) 
	{
			state = right;
			speed = 0.1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
			state = up;
			speed = 0.1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
			state = down;
			speed = 0.1;
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy)
{
	for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'e'))//���� ������� ������ �����
			{
				if (Dy > 0) { y = i * 32 - h;  dy = 0; }//�� Y 
				if (Dy < 0) { y = i * 32 + 32; dy = 0; }//������������ � �������� ������ 
				if (Dx > 0) { x = j * 32 - w; dx = 0; }//� ������ ����� �����
				if (Dx < 0) { x = j * 32 + 32; dx = 0; }// � ����� ����� �����
			}

		}
}

void Player::update(float time) //����� "���������/����������" ������� ������.
	{
		if (life)
		{//���������, ��� �� �����
			control();//������� ���������� ����������
			switch (state)//�������� ��������� �������� � ����������� �� ���������
			{
				case right:
				{//��������� ���� ������
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 77, 77, 80));
					break;
				}
				case left:
				{//��������� ���� �����
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 158, 77, 80));
					break;
				}
				case up:
				{//���� �����
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 77, 80));
					break;
				}
				case down:
				{//���� ����
					dy = speed;
					dx = 0;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 240, 77, 80));
					break;
				}
				case stay:
				{//�����
					dy = speed;
					dx = speed;
					break;
				}
			}

			x += dx*time; //�������� �� �X�
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			y += dy*time; //�������� �� �Y�
			checkCollisionWithMap(0, dy);//������������ ������������ �� Y

			speed = 0;    //�������� ��������, ����� �������� �����������.
			//state = stay;

			sprite.setPosition(x, y); //������ � ������� (x, y). 
		}
	}