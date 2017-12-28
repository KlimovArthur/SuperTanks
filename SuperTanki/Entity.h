#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"

#ifndef __ENTITY_H__
#define __ENTITY_H__


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

#endif //__ENTITY_H__