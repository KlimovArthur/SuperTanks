#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //���������� ��� � ������
#include <list>


using namespace sf;


class Entity {
public:
	enum { left, right, up, down, stay} state;// ��� ������������ - ��������� �������
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health; //���������� �health�, �������� ����� ������
	bool life; //���������� �life� �����, ����������
	bool check;
	int direction;
	Texture texture;//���� ��������
	Sprite sprite;//���� ������ 
	float CurrentFrame;//������ ������� ����
	std::string name;//����� ����� ���� ������, ������ ����� ��������� �� ������
	//������� ����� ���� ���� �������� � update() � ����������� �� �����



	Entity(Image &image, float X, float Y, int W, int H, std::string Name)
	{
		x = X; y = Y; //���������� ��������� �������
		w = W; h = H; 
		name = Name; 
		moveTimer = 0;
		dx = 0; dy = 0; 
		speed = 0;

		CurrentFrame = 0;
		health = 100;
		life = true; //���������������� ���������� ���������� �����, ����� ���
		texture.loadFromImage(image); //������� ���� ����������� � ��������
		sprite.setTexture(texture); //�������� ������ ���������
	}

	FloatRect getRect()
	{//����� ��������� ��������������. ��� �����, ������� (���,�����).
		FloatRect FR(x, y, w, h); // ���������� FR ���� FloatRect
		return FR;
		//��� ������ (�����) "sf::FloatRect" ��������� ������� ������ ���������� ��������������
		//� ����� ���� ��� ���������� �������� ������������ ����� �� �����
		//����� ��� �������� ��������, ���� �� ��� �����-���� ���� �� ���� ����� 
		//��� �-��� ����� ��� �������� ����������� 
	}

	
	virtual void update(float time) = 0;

};

class Player :public Entity {
public:
	int playerScore;//��� ���������� ����� ���� ������ � ������

	Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
	{
		playerScore = 0; 
		state = stay;
		if (name == "Player1")
		{
			//������ ������� ���� ������������� ���
			//������ ������ ������. IntRect � ��� ���������� �����
			sprite.setTextureRect(IntRect(0, 0, 77, 80));
		}
	}

	void control()
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

//����� �������� ������������ � ���������� �����
void checkCollisionWithMap(float Dx, float Dy)	{
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

void update(float time) //����� "���������/����������" ������� ������.
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

		if (health <= 0){ life = false; }//���� ������ ������ 0, ���� ����� 0, �� ������� 
		}
	}
};

class Enemy :public Entity{
public:
	int direction;//����������� �������� �����
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
	{
		if (name == "EasyEnemy")
		{
			//������ ������� ���� ������������� ���
			//������ ������ ������. IntRect � ��� ���������� �����
			sprite.setTextureRect(IntRect(0, 0, w, h));
			direction = rand() % (3); //����������� �������� ����� ����� ��������� �������
				//����� ��������� ��������� �����
				speed = 0.2;//���� ��������.���� ������ ������ ���������
				dx = speed;
				dy = speed;
		}
	}

void checkCollisionWithMap(float Dx, float Dy)//�-��� �������� ������������ � ������
{
	for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'e'))//���� ������� - ������ �����
			{
				if (Dy > 0) 
				{
					y = i * 32 - h;  dy = -0.1; 
					direction = rand() % (4); //����������� �������� �����
				}//�� Y 
				if (Dy < 0)
				{
					y = i * 32 + 32; dy = 0.1; 
					direction = rand() % (4);//����������� �������� ����� 
				}//������������ � �������� ������ 
				if (Dx > 0)
				{
					x = j * 32 - w; dx = -0.1; 
					direction = rand() % (4);//����������� �������� ����� 
				}//� ������ ����� �����
				if (Dx < 0)
				{
					x = j * 32 + 32; dx = 0.1; 
					direction = rand() % (4); //����������� �������� �����
				}// � ����� ����� �����
			}
		}
	}

	void update(float time)
	{
		if (name == "EasyEnemy")
		{//��� ��������� � ����� ������ ������ ����� �����
			if (life) 
			{//���������, ��� �� �����
				switch (direction)//�������� ��������� �������� � ����������� �� ���������
				{
					case 0:
					{//��������� ���� ������
						dx = speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 77, 77, 80));
						state = right;
						break;
					}
					case 1:
					{//��������� ���� �����
						dx = -speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 158, 77, 80));
						state = left;
						break;
					}
					case 2:
					{//���� �����
						dy = -speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 77, 80));
						state = up;
						break;
					}
					case 3:
					{//���� ����
						dy = speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 240, 77, 80));
						state = down;
					break;
					}
				}
				x += dx*time; //�������� �� �X�
				checkCollisionWithMap(dx, 0);//������������ ������������ �� �
				y += dy*time; //�������� �� �Y�
				checkCollisionWithMap(0, dy);//������������ ������������ �� Y

				sprite.setPosition(x, y); //������ � ������� (x, y).

				if (health <= 0)
				{ 
					life = false; 
				}//���� ������ ������ 0, ���� ����� 0, �� �������
			}
		}
	}
};
 
class Bullet :public Entity{//����� ����
public:
	int direction;//����������� ����
	//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir) 
	//��� ������� ����������� ����� ����
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2) :Entity(image, X, Y, W, H, Name)
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


	void update(float time)
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
};

class Base :public Entity 
{ 
public: 
Base(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name) 
{ 
x = X; 
y = Y; 
w = W; 
h = H; 
life = true; 
sprite.setTextureRect(IntRect(0, 0, 51, 65)); 
sprite.setPosition(x, y); 
}; 
void update(float time) {}; 
}; 

class Wall :public Entity 
{ 
public: 
Wall(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name) 
{ 
x = X; 
y = Y; 
w = W; 
h = H; 
life = true; 
sprite.setTextureRect(IntRect(160, 0, w, h)); 
sprite.setPosition(x, y); 

}; 
void update(float time) {}; 
};

class Star :public Entity 
{ 
public: 
Star(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name) 
{ 
x = X; 
y = Y; 
w = W; 
h = H; 
life = true; 
sprite.setTextureRect(IntRect(0, 0, w, h)); 
sprite.setPosition(x, y); 

}; 
void update(float time) {}; 
};
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
	std::list<Entity*> Walls; //������ ����
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator wall;
	std::list<Entity*>::iterator bullet;


	const int ENEMY_COUNT = 7; //������������ ���������� ������ � ���� �����!
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 100;
		float yr = 480;
		// ��������� ���������� ����� �� ���� ���� �� ��� �x�
		//������� ������ � �������� � ������
		enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
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



	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asMilliseconds();
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
