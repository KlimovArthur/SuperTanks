#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //���������� ��� � ������
#include <list>
#include "classes.h"
#include "ourclasses.h"

using namespace sf;


int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1280, 704, desktop.bitsPerPixel), "SuperTanki");

	Font font;//����� 
	font.loadFromFile("xenoa.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 180);//������� ������ �����
	text.setColor(Color::Red);//��������� ����� � �������	
	text.setStyle(Text::Bold);//������ �����.

	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	Clock clock;
	Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ���� 
	int gameTime = 0;//�������� ������� �����, ����������������.
	int delay = 0;
	bool winCheck = false;
	bool starCheck = false;
	int i = 0;


	Image heroImage;
	heroImage.loadFromFile("images/TankPlayer.png"); // ��������� ����������� ������

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/TankEnemy.png"); // ��������� ����������� �����

	Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������

	Image BaseImage;//����������� ��� ����
	BaseImage.loadFromFile("images/bird.png");//��������� �������� � ������ �����������

	Image StarImage;
	StarImage.loadFromFile("images/star.png");

	Player p(heroImage, 1100, 70, 70, 70, "Player1");//������ ������ ������

	Base b(BaseImage, 1150, 332, 71, 65, "Base");

	Star *s = new Star(StarImage, 150, 510, 63, 44, "Star");

	std::list<Entity*>  enemies; //������ ������
	std::list<Entity*>  Bullets; //������ ����
	std::list<Entity*>  Walls; //������ ����
	std::list<Entity*>::iterator it; //�������� ����� ��������� �� ��������� ������
	std::list<Entity*>::iterator it2;
	std::list<Entity*>::iterator bullet;
	std::list<Entity*>::iterator wall;

	const int ENEMY_COUNT = 7;	//������������ ���������� ������ � ���� �����!
	int enemiesCount = 0;	//������� ���������� ������ � ����

	//��������� ������ ��������� �������
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 100;
		float yr = 480;
		 // ��������� ���������� ����� �� ���� ���� �� ��� �x�
	

		//������� ������ � �������� � ������
		enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
		enemiesCount += 1; //��������� ������� ������
	}

	Walls.push_back(new Wall(map_image, 1184, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1152, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1120, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1088, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1088, 320, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1088, 352, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1088, 384, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1088, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1120, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1152, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 1184, 416, 32, 32, "Wall"));

	int createObjectForMapTimer = 0;//���������� ��� ����� ��� ������������� ������

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asMilliseconds();//������� ����� � 
		//�������� ��� ������, ���� ��� �����. ������������� ��� time ��� �� ����. 
		//��� �� ��������� ������ ����
		clock.restart();
		time = time / 800;

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//�������� �� ������� ������� "Space"
			if ((event.type == sf::Event::KeyPressed) && (p.life == true))
			{
				if ((event.key.code == sf::Keyboard::Space))
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state, true));
					
				}
			}

		}

		p.update(time); //�������� ������ �p� ������ �Player� 

		//�������� ������
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
		}
		
		delay = delay + 1;

		if (delay == 500)
		{
			for (it = enemies.begin(); it != enemies.end(); it++)
			{
				if (rand() % 2 == 0)
				{
					Bullets.push_back(new Bullet(BulletImage, (*it)->x, (*it)->y, 16, 16, "Bullet", (*it)->state, false));
				}
			}
			delay = 0;
		}
		//�������� ����
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
		}
		//��������� ������ �� ������� "�������" ���� � ������� ��
		for (it = Bullets.begin(); it != Bullets.end(); )//������� ��� ���������� �� ������ �� �����
		{// ���� ���� ������ �����, �� ������� ���
			if ((*it)-> life == false)	
			{ 
				it = Bullets.erase(it); 
			} 
			else  it++;//� ���� �������� (����������) � ���� �������.		
		}

		for (bullet = Bullets.begin();  bullet!= Bullets.end(); bullet++)
		{
			for (it = enemies.begin(); it != enemies.end(); )
			{//����� �� ������ ������
				if (((*bullet)->getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy") && ((*bullet)->check == true))
				{
					it = enemies.erase(it);
					(*bullet)->life = false;
				}
				else it++;
			}

			if (p.getRect().intersects((*bullet)->getRect()) && ((*bullet)->check == false))
			{
				p.life = false;
			}

			if (b.getRect().intersects((*bullet)->getRect()))
			{
				b.life = false;
				p.life = false;
			}

			for (it = Walls.begin(); it != Walls.end(); )
			{
				if ((*bullet)->getRect().intersects((*it)->getRect()))
				{
					it = Walls.erase(it);
					(*bullet)->life = false;
				}
				else it++;
			}
		}


		for (it = enemies.begin(); it != enemies.end(); it++) 
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				p.life = false;
			}
		}

		

		if (p.getRect().intersects(s->getRect()) && (starCheck == false))
		{
			for (it = enemies.begin(); it != enemies.end(); it++) 
			{
				(*it)->speed = 0;
			}
			delete s;
			starCheck = true;
		}

	window.clear();
/////////////////////////////������ �����/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(32, 0, 32, 32));
			if ((TileMap[i][j] == ' ') || (TileMap[i][j] == 'e')) s_map.setTextureRect(IntRect(0, 0, 32, 32));
			s_map.setPosition(j * 32, i * 32);
			window.draw(s_map);
		}
		if (((p.life == false) || (b.life == false)) && (winCheck == false))
		{
				text.setString("You lost");//������ ������ ������
				text.setPosition(330 ,250);//������ ������� ������
				window.draw(text);//������ ���� �����
		}
		if (enemies.empty())
		{
				winCheck = true;
				text.setString("You won");//������ ������ ������
				text.setPosition(340 ,250);//������ ������� ������
				window.draw(text);//������ ���� �����
		}

		window.draw(p.sprite);//������ ������ ������� �p� ������ �Player�
		window.draw(b.sprite);
		if (starCheck == false)
		{
			window.draw(s->sprite);
		}
		//������ ������
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			if ((*it)->life) //���� ����� ����
			window.draw((*it)->sprite); //������ 
		}

		//������ ����
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			if ((*it)->life) //���� ���� ����
				window.draw((*it)->sprite); //������ �������
		}

		for (it = Walls.begin(); it != Walls.end(); it++)
		{
			window.draw((*it)->sprite);
		}

		window.display();
	} 
	return 0;
}