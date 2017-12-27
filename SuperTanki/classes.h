#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"

#ifndef __CCLASSES_H__
#define __CCLASSES_H__


using namespace sf;

class Entity {
public:
	enum { left, right, up, down, stay} state;// ��� ������������ - ��������� �������
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h; //���������� �health�, �������� ����� ������
	bool life; //���������� �life� �����, ����������
	bool check;
	int direction;
	Texture texture;//���� ��������
	Sprite sprite;//���� ������ 
	float CurrentFrame;//������ ������� ����
	std::string name;//����� ����� ���� ������, ������ ����� ��������� �� ������
	//������� ����� ���� ���� �������� � update() � ����������� �� �����

	Entity(Image &image, float X, float Y, int W, int H, std::string Name);

	FloatRect getRect();
	
	virtual void update(float time) = 0;

};

class Player :public Entity 
{
	public:
		Player(Image &image, float X, float Y, int W, int H, std::string Name);
		void control();
		//����� �������� ������������ � ���������� �����
		void checkCollisionWithMap(float Dx, float Dy);

		void update(float time);
};

class Enemy :public Entity{
public:
	int direction;//����������� �������� �����
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name);
	void checkCollisionWithMap(float Dx, float Dy);//�-��� �������� ������������ � ������
	void update(float time);
};

class Bullet :public Entity{//����� ����
public:
	int direction;//����������� ����
	//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir) 
	//��� ������� ����������� ����� ����
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2);
	void update(float time);
};

#endif //__CCLASSES_H__