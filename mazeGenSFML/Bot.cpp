#include "Bot.h"



using namespace sf;
using namespace std;

vector<cell> Bot::validCells;
vector<String> Bot::validFiles;
vector<String> Bot::validNames;

Bot::Bot()
{
	//�����������

	//���������� ������� ��� ����:	
	//

	if (validFiles.size() == 0)
	{
		//����� �����, �������� �� ���� �� �������
		Bot::prepareSkins();
	}
	int randNum = rand() % validFiles.size();
	tileset = "Images/BotTilesets/" + validFiles[randNum];
	validFiles.erase(validFiles.begin() + randNum);

	if (validNames.size() == 0)
	{
		//����� �����, ��� ���� �� �������
		Bot::prepareNames();
	}
	randNum = rand() % validNames.size();
	name = validNames[randNum];
	validNames.erase(validNames.begin() + randNum);
	
	image.loadFromFile(tileset);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, 16, 16));

	frags = 0;
	deaths = 0;
	alive = true;
	respawnPause = 10000;
	currentPause = 0;
	timeAlive = 0;
	speed = 0.2;
	dir = 0;
	strength = 5;

}

void Bot::generatePosition()
{
	//�������� ����� ������ ���������
	//srand(time(0));
	cell tmp = validCells[rand() % validCells.size()];
	x = tmp.x;
	y = tmp.y;
	X = 16 * x;
	Y = 16 * y;
	Xcenter = X + w / 2;
	Ycenter = Y + h / 2;
}

void Bot::respawn(Maze &mz)
{
	alive = true;
	generatePosition();
	navigate(mz);
	//��������� ���� � ��� �������, ���������� ���
	sprite.setPosition(x*w, y*h);
}

void Bot::update(float time, Maze &mz)
{
	if (alive)
	{
		switch (dir)
		{
		case down:
			//����
			sprite.setTextureRect(IntRect(0, 0, 16, 16));//��������� ����� �� ����������� ��������
			dy = speed;
			dx = 0;
			break;
		case up:
			//�����
			sprite.setTextureRect(IntRect(0, 17, 16, 16));
			dy = -speed;
			dx = 0;
			break;
		case dleft:
			//�����
			sprite.setTextureRect(IntRect(17, 17, 16, 16));
			dx = -speed;
			dy = 0;
			break;
		case dright:
			//������
			sprite.setTextureRect(IntRect(17, 0, 16, 16));
			dx = speed;
			dy = 0;
			break;
		default:
			break;
		}
		prevX = X;
		prevY = Y;
		float fixTime = 5.0;
		if (time < 10)
		{
			if (int((X + dx*time) / 16) < mz.width - 2 && int((X + dx*time) / 16) > 0)
				X += dx*time;
			if (int((Y + dy*time) / 16) < mz.height - 2 && int((Y + dy*time) / 16) > 0)
				Y += dy*time;
		}
		else
		if (time > 5)
		{
			//����� ��� �����������
			X += dx*fixTime;
			Y += dy*fixTime;
		}

		int tmpx = x, tmpy = y;
		x = X / 16;
		y = Y / 16;
		Xcenter = X + w / 2;
		Ycenter = Y + h / 2;
		//cout << "time passed: " << time << "; Xpos = " << X << "; Ypos = " << Y << "\n";

		//������: ���� ������� ����� ������� ���������� � ������ ������ ��� �������� - ��������� - �������� �Ө��
		//����������� ��� ������� �����������
		switch (dir)
		{
		case up:
			if (abs(Y - y * 16) < 8)
			{
				Y = y * 16;
			}
			break;
		case down:
			if (abs((y + 1) * 16 - Y) < 8)
			{
				Y = (y + 1) * 16;
				y = Y / 16;
			}
			break;
		case dleft:
			if (abs(X - x * 16) < 8)
			{
				X = x * 16;
			}
			break;
		case dright:
			if (abs((x + 1) * 16 - X) < 8)
			{
				X = (x + 1) * 16;
				x = X / 16;
			}
			break;
		}
		/*if (x * 16 - X < 1)
			X = x * 16;
		if (y * 16 - Y < 1)
			Y = y * 16;*/

		mazeInteraction(mz);
		//if ( int((X+w/2)/16) == x && int((Y+h/2)/16) == y)
		//{
		//	//������, ������ ���� ������ � ������������ ���������
		//	//� ���� ����������, ���� ��������� ������
		//	navigate(mz);
		//}
		//if (X % 16 == 0 && Y % 16 == 0)
		if ((X - int(X) == 0 && int(X) % 16 == 0) && (Y - int(Y) == 0 && int(Y) % 16 == 0))
			navigate(mz);
		//���� �� ������ ������ - ������, ���� ��������, ���� ��������� � �����
		
		sprite.setPosition(X, Y);
	}
	else
	{
		//���������, ������ �� ����� �� ��������
		currentPause -= time;
		if (currentPause < 0)
			respawn(mz);
	}
}

void Bot::eat()
{
	//��� ������� �������
	//other.die();
	frags++;
}

void Bot::die()
{
	//������
	currentPause = respawnPause;
	alive = false;
	deaths++;
}

void Bot::prepareValidCellsList(Maze &mz)
{
	//��������� ���� �������� � ����������� � ������ ��� ������ ������
	for (int i = 0; i < mz.height; i++)
	{
		for (int j = 0; j < mz.width; j++)
		{
			if (mz.maze[i][j] != '*')
			{
				cell temp = { j, i };
				validCells.insert(validCells.end(), temp);
			}
		}
	}
}

void Bot::prepareSkins()
{
	validFiles = {
		"deep-blue.png",
		"purple-sprite.png",
		"green-sprite.png",
		"red-sprite.png",
		"red-cyclop.png",
		"blue-cyclop.png",
		"glamor.png",
		"green-cyclop.png",
		"green-mutant.png",
		"green-scared.png",
		"orange-cyclop.png",
		"orange-mutant.png",
		"orange-scared.png",
		"pink-cyclop.png",
		"pink-scared.png", 
		"red-mutant.png",
		"red-scared.png"
	};
}

void Bot::prepareNames()
{
	validNames = {
		"Henry",
		"Joey",
		"Sally",
		"Perkins",
		"Curtis",
		"Sandy",
		"Johnny",
		"Atom",
		"Katy",
		"Tommy",
		"Carl",
		"Smokey",
		"Marty",
		"Byte",
		"Ryder",
		"Billy",
		"Blondie"
	};
}


void Bot::navigate(Maze &mz)
{
	//������������ ��������� ���� �� ���������
	//���� �� �������� - ��� ����-��
	validDirs Dir;
	Dir.size = 0;
	if (mz.maze[y - 1][x] != '*')
	{
		Dir.size++;
		Dir.dirs[Dir.size - 1] = up;
	}
	if (mz.maze[y + 1][x] != '*')
	{
		Dir.size++;
		Dir.dirs[Dir.size - 1] = down;
	}
	if (mz.maze[y][x + 1] != '*')
	{
		Dir.size++;
		Dir.dirs[Dir.size - 1] = dright;
	}
	if (mz.maze[y][x - 1] != '*')
	{
		Dir.size++;
		Dir.dirs[Dir.size - 1] = dleft;
	}
	//���� ������� � �����
	//if (round(prevX) == round(X) || round(prevY) == round(Y))
	//{
	//	X = round(X);
	//	Y = round(Y);
	//	//dir = Dir.dirs[rand() % Dir.size];
	//}
	if (Dir.size == 2)
	{
		//��� ������: ���� ��� �������, ���� ��� �������, �� �������� ���
		if (abs(Dir.dirs[0] - Dir.dirs[1]) != 2)
		{
			//�������� ��������: ���� ������� �� ����� ����, ������, �������� ������ �� �������������� ���� �����
			if (dir + 2 == Dir.dirs[0] || dir - 2 == Dir.dirs[0])
				dir = Dir.dirs[1];
			else
				dir = Dir.dirs[0];
		}
		//����� - �������, �� �������� �� ��� ���, ����������� �� ��������
		//���������, ��� �� ������ �� ���� ��� � ����� �������
		if (dir != Dir.dirs[0] && dir != Dir.dirs[1])
			dir = Dir.dirs[rand() % Dir.size];
	}
	else if (Dir.size == 1)
	{
		//�����.
		dir = Dir.dirs[0];
	}
	else if (Dir.size > 2)
	{
		//��������
		//���������� ��������� �����������
		dir = Dir.dirs[rand() % Dir.size];
	}

}

void Bot::mazeInteraction(Maze &mz)
{
	for (int i = Y / 16; i < (Y + h) / 16; i++)
	{
		for (int j = X / 16; j < (X + w) / 16; j++)
		{
			
			if (mz.maze[i][j] == '*')
			{
				if (dx > 0)
					X = j * 16 - w;
				if (dx < 0)
					X = j * 16 + 16;
				if (dy > 0)
					Y = i * 16 - h;
				if (dy < 0)
					Y = i * 16 + 16;
			}
		}
	}
}
