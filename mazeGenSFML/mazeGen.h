#ifndef MAZE
#define MAZE

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "struct.h"


using namespace sf;


class Maze
{
public:
	int width, height, seed; //ширина и высота лабиринта, сид гсч
	char ** maze;
	int entranceSide, exitSide; //стороны, на которых расположены входы и выходы

	int entranceX, entranceY, exitX, exitY;
	int currentX, currentY;
	bool ** visited; //матрица посещённых и непосещённых клеток
	int temp; //а сюда будем мусор пихать
	int visitedCells; //посещённые клетки
	cell currentCell, prevCell;
	cellPack nearby;
	bool flag;

	Image mapImg;//изображение карты

	Texture mapTex;//текстура карты

	Sprite mapSpr;

	Maze(int W, int H, int S, String tileset)
	{
		mapImg.loadFromFile(tileset);

		mapTex.loadFromImage(mapImg);

		mapSpr.setTexture(mapTex);
		width = W;
		height = H;
		seed = S;
	}
	void generateMaze();
	void draw(RenderWindow& window, String tileset);
};

#endif