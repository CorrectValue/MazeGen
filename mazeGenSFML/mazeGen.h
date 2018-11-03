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
	int width, height, seed; //������ � ������ ���������, ��� ���
	char ** maze;
	int entranceSide, exitSide; //�������, �� ������� ����������� ����� � ������

	int entranceX, entranceY, exitX, exitY;
	int currentX, currentY;
	bool ** visited; //������� ���������� � ������������ ������
	int temp; //� ���� ����� ����� ������
	int visitedCells; //���������� ������
	cell currentCell, prevCell;
	cellPack nearby;
	bool flag;

	Image mapImg;//����������� �����

	Texture mapTex;//�������� �����

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