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

int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1280, 704, desktop.bitsPerPixel), "SuperTanki");
	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Clock clock;
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
	int gameTime = 0;//объявили игровое время, инициализировали.

	Image heroImage;
	heroImage.loadFromFile("images/TankPlayer.png"); // загружаем изображение игрока
	Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/TankEnemy.png"); // загружаем изображение врага
	Player p(heroImage, 1100, 70, 70, 70, "Player1");//объект класса игрока
	std::list<Entity*>  enemies; //список врагов
	std::list<Entity*>::iterator it;
	const int ENEMY_COUNT = 7; //максимальное количество врагов в игре ВАЖНО!
 int enemiesCount = 0; //текущее количество врагов в игре
	for (int i = 0; i < ENEMY_COUNT; i++)
 {
  float xr = 150 + rand() % 100;
  float yr = 480;
   // случайная координата врага на поле игры по оси “x”
 

  //создаем врагов и помещаем в список
  enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
  enemiesCount += 1; //увеличили счётчик врагов
 }
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
		}


		p.update(time); //оживляем объект “p” класса “Player”
		for (it = enemies.begin(); it != enemies.end(); it++) 
       { 
       (*it)->update(time); //запускаем метод update() 
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

		window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
		for (it = enemies.begin(); it != enemies.end(); it++) 
           { 
             if ((*it)->life) //если враги живы 
             window.draw((*it)->sprite); //рисуем 
           }
		window.display();
	}
	return 0;
}
