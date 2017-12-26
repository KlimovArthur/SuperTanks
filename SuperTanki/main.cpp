#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //подключили код с картой
#include <list>


using namespace sf;


class Entity {
public:
	enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health; //переменная “health”, хранящая жизни игрока
	bool life; //переменная “life” жизнь, логическая
	bool check;
	int direction;
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт 
	float CurrentFrame;//хранит текущий кадр
	std::string name;//враги могут быть разные, врагов можно различать по именам
	//каждому можно дать свое действие в update() в зависимости от имени



	Entity(Image &image, float X, float Y, int W, int H, std::string Name)
	{
		x = X; y = Y; //координата появления спрайта
		w = W; h = H; 
		name = Name; 
		moveTimer = 0;
		dx = 0; dy = 0; 
		speed = 0;

		CurrentFrame = 0;
		health = 100;
		life = true; //инициализировали логическую переменную жизни, герой жив
		texture.loadFromImage(image); //заносим наше изображение в текстуру
		sprite.setTexture(texture); //заливаем спрайт текстурой
	}

	FloatRect getRect()
	{//метод получения прямоугольника. его коорд, размеры (шир,высот).
		FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
		return FR;
		//Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
		//в нашей игре это координаты текущего расположения тайла на карте
		//далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте 
		//эта ф-ция нужна для проверки пересечений 
	}

	
	virtual void update(float time) = 0;

};

class Player :public Entity {
public:
	int playerScore;//эта переменная может быть только у игрока

	Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
	{
		playerScore = 0; 
		state = stay;
		if (name == "Player1")
		{
			//Задаем спрайту один прямоугольник для
			//вывода одного игрока. IntRect – для приведения типов
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

//Метод проверки столкновений с элементами карты
void checkCollisionWithMap(float Dx, float Dy)	{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'e'))//если элемент тайлик земли
			{
				if (Dy > 0) { y = i * 32 - h;  dy = 0; }//по Y 
				if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями 
				if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
				if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
			}

		}
	}

void update(float time) //метод "оживления/обновления" объекта класса.
	{
		if (life)
		{//проверяем, жив ли герой
			control();//функция управления персонажем
			switch (state)//делаются различные действия в зависимости от состояния
			{
				case right:
				{//состояние идти вправо
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 77, 77, 80));
					break;
				}
				case left:
				{//состояние идти влево
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 158, 77, 80));
					break;
				}
				case up:
				{//идти вверх
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 77, 80));
					break;
				}
				case down:
				{//идти вниз
					dy = speed;
					dx = 0;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 240, 77, 80));
					break;
				}
				case stay:
				{//стоим
					dy = speed;
					dx = speed;
					break;
				}
			}

			x += dx*time; //движение по “X”
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			y += dy*time; //движение по “Y”
			checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

			speed = 0;    //обнуляем скорость, чтобы персонаж остановился.
			//state = stay;

			sprite.setPosition(x, y); //спрайт в позиции (x, y).

		if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем 
		}
	}
};

class Enemy :public Entity{
public:
	int direction;//направление движения врага
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
	{
		if (name == "EasyEnemy")
		{
			//Задаем спрайту один прямоугольник для
			//вывода одного игрока. IntRect – для приведения типов
			sprite.setTextureRect(IntRect(0, 0, w, h));
			direction = rand() % (3); //Направление движения врага задаём случайным образом
				//через генератор случайных чисел
				speed = 0.2;//даем скорость.этот объект всегда двигается
				dx = speed;
				dy = speed;
		}
	}

void checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'e'))//если элемент - тайлик земли
			{
				if (Dy > 0) 
				{
					y = i * 32 - h;  dy = -0.1; 
					direction = rand() % (4); //Направление движения врага
				}//по Y 
				if (Dy < 0)
				{
					y = i * 32 + 32; dy = 0.1; 
					direction = rand() % (4);//Направление движения врага 
				}//столкновение с верхними краями 
				if (Dx > 0)
				{
					x = j * 32 - w; dx = -0.1; 
					direction = rand() % (4);//Направление движения врага 
				}//с правым краем карты
				if (Dx < 0)
				{
					x = j * 32 + 32; dx = 0.1; 
					direction = rand() % (4); //Направление движения врага
				}// с левым краем карты
			}
		}
	}

	void update(float time)
	{
		if (name == "EasyEnemy")
		{//для персонажа с таким именем логика будет такой
			if (life) 
			{//проверяем, жив ли герой
				switch (direction)//делаются различные действия в зависимости от состояния
				{
					case 0:
					{//состояние идти вправо
						dx = speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 77, 77, 80));
						state = right;
						break;
					}
					case 1:
					{//состояние идти влево
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
				x += dx*time; //движение по “X”
				checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
				y += dy*time; //движение по “Y”
				checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

				sprite.setPosition(x, y); //спрайт в позиции (x, y).

				if (health <= 0)
				{ 
					life = false; 
				}//если жизней меньше 0, либо равно 0, то умираем
			}
		}
	}
};
 
class Bullet :public Entity{//класс пули
public:
	int direction;//направление пули
	//всё так же, только взяли в конце состояние игрока (int dir) 
	//для задания направления полёта пули
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2) :Entity(image, X, Y, W, H, Name)
	{
		x = X+20;
		y = Y+5;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		check = check2;
		//выше инициализация в конструкторе
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
			x += dx*time;//само движение пули по х
			y += dy*time;//по у

		if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
		if (y <= 0) y = 20;

		if (x >= 1280) x = 1260;// задержка пули в правой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
		if (y >= 704) y = 684;


			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
						life = false;// то пуля умирает
				}
			sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
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


	Font font;//шрифт 
	font.loadFromFile("xenoa.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 180);//создаем объект текст
	text.setColor(Color::Red);//покрасили текст в красный	
	text.setStyle(Text::Bold);//жирный текст.


	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом



	Clock clock;
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
	int gameTime = 0;//объявили игровое время, инициализировали.
	int delay = 0;
	bool winCheck = false; 
    bool starCheck = false; 
    int i = 0;



	Image heroImage;
	heroImage.loadFromFile("images/TankPlayer.png"); // загружаем изображение игрока

	Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/TankEnemy.png"); // загружаем изображение врага

	Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения

	Image BaseImage;//изображение для пули 
    BaseImage.loadFromFile("images/bird.png");//загрузили картинку в объект изображения 

	Image StarImage; 
    StarImage.loadFromFile("images/star.png");



	Player p(heroImage, 1100, 70, 70, 70, "Player1");//объект класса игрока
    Base b(BaseImage, 1150, 332, 71, 65, "Base");
	Star *s = new Star(StarImage, 150, 510, 63, 44, "Star");


	std::list<Entity*>  enemies; //список врагов
	std::list<Entity*>  Bullets; //список пуль
	std::list<Entity*> Walls; //список стен
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator wall;
	std::list<Entity*>::iterator bullet;


	const int ENEMY_COUNT = 7; //максимальное количество врагов в игре ВАЖНО!
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 100;
		float yr = 480;
		// случайная координата врага на поле игры по оси “x”
		//создаем врагов и помещаем в список
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
			//стреляем по нажатию клавиши "Space"
			if ((event.type == sf::Event::KeyPressed) && (p.life == true))
			{
				if ((event.key.code == sf::Keyboard::Space))
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state, true));
					
				}
			}
		}



		p.update(time); //оживляем объект “p” класса “Player”
		for (it = enemies.begin(); it != enemies.end(); it++) 
        { 
			(*it)->update(time); //запускаем метод update() 
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



		//оживляем пули
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //запускаем метод update()
		}


		//Проверяем список на наличие "мертвых" пуль и удаляем их
		for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
		{// если этот объект мертв, то удаляем его
			if ((*it)-> life == false)	
			{ 
				it = Bullets.erase(it); 
			} 
			else  it++;//и идем курсором (итератором) к след объекту.		
		}



		for (bullet = Bullets.begin();  bullet!= Bullets.end(); bullet++)
		{
			for (it = enemies.begin(); it != enemies.end(); )
			{//бежим по списку врагов
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
/////////////////////////////Рисуем карту/////////////////////
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
				text.setString("You lost");//задаем строку тексту
				text.setPosition(330 ,250);//задаем позицию текста
				window.draw(text);//рисуем этот текст
		}
		if (enemies.empty())
		{
				winCheck = true;
				text.setString("You won");//задаем строку тексту
				text.setPosition(340 ,250);//задаем позицию текста
				window.draw(text);//рисуем этот текст
		}
		window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
		window.draw(b.sprite);

		if (starCheck == false) 
        { 
              window.draw(s->sprite); 
        }
		for (it = enemies.begin(); it != enemies.end(); it++) 
        { 
			if ((*it)->life) //если враги живы 
			window.draw((*it)->sprite); //рисуем 
        }


		//рисуем пули
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			if ((*it)->life) //если пули живы
				window.draw((*it)->sprite); //рисуем объекты
		}
		for (it = Walls.begin(); it != Walls.end(); it++) 
		{ 
			window.draw((*it)->sprite); 
		}
		window.display();
	}
	return 0;
}
