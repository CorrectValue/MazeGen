#ifndef BOT
#define BOT

#include <SFML/Graphics.hpp>
#include "mazeGen.h"
#include "struct.h"
#include <vector>


using namespace sf;
using namespace std;




class Bot
{
	//определ€ет свойства и поведение бота
public:
	static vector<cell> validCells;
	static vector<String> validFiles;
	static vector<String> validNames;

	String tileset; 
	Image image;//сфмл изображение
	sf::Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	int dir; //направление движени€ бота

	float speed; //скорость движени€ ботинка
	int w = 16, h = 16;

	int frags, deaths; //счЄт бота: количество убийств и смертей
	float timeAlive; //врем€ жизни
	int x, y; //координаты в пространстве лабиринта
	float X, Y; //координаты в пространстве окна
	float Xcenter, Ycenter;
	float prevX, prevY;
	float dx, dy;

	String name;

	int strength; //сила бота определ€ет веро€тность его победы в схватке

	float respawnPause, currentPause; //врем€ паузы перед респавном

	bool alive;

	Bot();
	void mazeInteraction(Maze mz);
	void respawn(Maze mz); //респавн бота в случайной точке карты

	void generatePosition();
	void update(float time, Maze mz);
	void eat();
	void die();
	void navigate(Maze mz);
	static void prepareValidCellsList(Maze mz);
	static void prepareSkins();
	static void prepareNames();

};

#endif