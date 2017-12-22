#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //подключили код с картой
#include <list>


using namespace sf;


int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1280, 704, desktop.bitsPerPixel), "SuperTanki");
	Image map_image;//объект изображени€ дл€ карты
	map_image.loadFromFile("images/map.png");//загружаем файл дл€ карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//зар€жаем текстуру картинкой
	Sprite s_map;//создаЄм спрайт дл€ карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Clock clock;
	Clock gameTimeClock;//переменна€ игрового времени, будем здесь хранить врем€ игры 
	int gameTime = 0;//объ€вили игровое врем€, инициализировали.



	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
/////////////////////////////–исуем карту/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(32, 0, 32, 32));
			if ((TileMap[i][j] == ' ') || (TileMap[i][j] == 'e')) s_map.setTextureRect(IntRect(0, 0, 32, 32));
			s_map.setPosition(j * 32, i * 32);
			window.draw(s_map);
		}
		window.display();
	}
	return 0;
}
