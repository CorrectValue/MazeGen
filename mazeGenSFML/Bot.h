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
	//���������� �������� � ��������� ����
public:
	static vector<cell> validCells;
	static vector<String> validFiles;
	static vector<String> validNames;

	String tileset; 
	Image image;//���� �����������
	sf::Texture texture;//���� ��������
	Sprite sprite;//���� ������

	int dir; //����������� �������� ����

	float speed; //�������� �������� �������
	int w = 16, h = 16;

	int frags, deaths; //���� ����: ���������� ������� � �������
	float timeAlive; //����� �����
	int x, y; //���������� � ������������ ���������
	float X, Y; //���������� � ������������ ����
	float Xcenter, Ycenter;
	float prevX, prevY;
	float dx, dy;

	String name;

	int strength; //���� ���� ���������� ����������� ��� ������ � �������

	float respawnPause, currentPause; //����� ����� ����� ���������

	bool alive;

	Bot();
	void mazeInteraction(Maze mz);
	void respawn(Maze mz); //������� ���� � ��������� ����� �����

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