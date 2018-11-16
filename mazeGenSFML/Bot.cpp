#include "Bot.h"



using namespace sf;
using namespace std;

vector<cell> Bot::validCells;
vector<String> Bot::validFiles;
vector<String> Bot::validNames;

Bot::Bot()
{
	//подготовить

	//рандомится внешний вид бота:	
	//

	if (validFiles.size() == 0)
	{
		//много ботов, картинок на всех не хватает
		Bot::prepareSkins();
	}
	int randNum = rand() % validFiles.size();
	tileset = "Images/BotTilesets/" + validFiles[randNum];
	validFiles.erase(validFiles.begin() + randNum);

	if (validNames.size() == 0)
	{
		//много ботов, имён всем не хватает
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
	//рандомит точку внутри лабуринта
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
	//поместить бота в эту позицию, отрисовать его
	sprite.setPosition(x*w, y*h);
}

void Bot::update(float time, Maze &mz)
{
	if (alive)
	{
		switch (dir)
		{
		case down:
			//вниз
			sprite.setTextureRect(IntRect(0, 0, 16, 16));//повернуть перса по направлению движения
			dy = speed;
			dx = 0;
			break;
		case up:
			//вверх
			sprite.setTextureRect(IntRect(0, 17, 16, 16));
			dy = -speed;
			dx = 0;
			break;
		case dleft:
			//влево
			sprite.setTextureRect(IntRect(17, 17, 16, 16));
			dx = -speed;
			dy = 0;
			break;
		case dright:
			//вправо
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
			//нужно для трассировки
			X += dx*fixTime;
			Y += dy*fixTime;
		}

		int tmpx = x, tmpy = y;
		x = X / 16;
		y = Y / 16;
		Xcenter = X + w / 2;
		Ycenter = Y + h / 2;
		//cout << "time passed: " << time << "; Xpos = " << X << "; Ypos = " << Y << "\n";

		//втупую: если разница между текущим положением и блоком меньше его половины - округляем - РАБОТАЕТ ХУЁВО
		//рассмотреть для четырех направлений
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
		//	//значит, прошли одну клетку в пространстве лабиринта
		//	//и надо определить, куда двигаться дальше
		//	navigate(mz);
		//}
		//if (X % 16 == 0 && Y % 16 == 0)
		if ((X - int(X) == 0 && int(X) % 16 == 0) && (Y - int(Y) == 0 && int(Y) % 16 == 0))
			navigate(mz);
		//если не прошли клетку - значит, либо движемся, либо влупились в стену
		
		sprite.setPosition(X, Y);
	}
	else
	{
		//проверить, прошло ли время до респавна
		currentPause -= time;
		if (currentPause < 0)
			respawn(mz);
	}
}

void Bot::eat()
{
	//жрёт другого ботинка
	//other.die();
	frags++;
}

void Bot::die()
{
	//смэрть
	currentPause = respawnPause;
	alive = false;
	deaths++;
}

void Bot::prepareValidCellsList(Maze &mz)
{
	//пробегаем весь лабиринт и запихуиваем в список все пустые клетки
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
	//осуществляет нафигацию бота по лабиринту
	//пока не развилка - идём куда-то
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
	//если упёрлись в стену
	//if (round(prevX) == round(X) || round(prevY) == round(Y))
	//{
	//	X = round(X);
	//	Y = round(Y);
	//	//dir = Dir.dirs[rand() % Dir.size];
	//}
	if (Dir.size == 2)
	{
		//два соседа: либо это поворот, либо это туннель, по которому идём
		if (abs(Dir.dirs[0] - Dir.dirs[1]) != 2)
		{
			//достигли поворота: если разница не равна двум, значит, соседние клетки не противоположны друг другу
			if (dir + 2 == Dir.dirs[0] || dir - 2 == Dir.dirs[0])
				dir = Dir.dirs[1];
			else
				dir = Dir.dirs[0];
		}
		//иначе - туннель, по которому мы уже идём, направление не меняется
		//проверить, идём мы вообще по нему или в стену лупимся
		if (dir != Dir.dirs[0] && dir != Dir.dirs[1])
			dir = Dir.dirs[rand() % Dir.size];
	}
	else if (Dir.size == 1)
	{
		//тупик.
		dir = Dir.dirs[0];
	}
	else if (Dir.size > 2)
	{
		//развилка
		//выбирается случайное направление
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
