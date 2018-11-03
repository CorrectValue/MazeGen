#include "stdafx.h"
#include "mazeGen.h"

void Maze::generateMaze()
{
	//далее наверн запросим с клавы размеры лабиринта


	//задать сид генерации
	srand(seed);

	//теперь нужно построить каркас лабуринта
	maze = new char*[height];

	for (int i = 0; i < height; i++)
	{
		maze[i] = new char[width];
	}

	//заведомо сделаем так: все доступные клетки разбиты стенами
	//рисуем сетку!
	for (int i = 0; i < height; i++)
	{
		//проходим по горизонтальным линиям
		if (i % 2 == 0)
		{
			//рисуем линию стен
			for (int j = 0; j < width; j++)
			{
				maze[i][j] = '*';
			}
		}
		else
			//рисуем линию пробелов
			for (int j = 0; j < width; j++)
			{
				maze[i][j] = ' ';
			}
	}

	//аналогично рисуем вертикальные линии
	for (int i = 0; i < width; i++)
	{
		if (i % 2 == 0)
		{
			//чётная линия - стена
			for (int j = 0; j < height; j++)
			{
				maze[j][i] = '*';
			}
		}
		else
			//нечётная линия - проход... учесть заранее сгенерированные линии!
			for (int j = 0; j < height; j++)
			{
				if (maze[j][i] != '*')
					maze[j][i] = ' ';
			}
	}

	//набить матрицу посещённых точек
	//посчитать, сколько осталось доступных точек вообще
	int validCellsX, validCellsY;
	validCellsX = width / 2; //округляется в меньшую сторону, да?
	validCellsY = height / 2;

	//инициализировать матрицу посещений
	visited = new bool*[validCellsY];
	for (int i = 0; i < validCellsY; i++)
		visited[i] = new bool[validCellsX];
	//изначально вся матрица не посещена
	for (int i = 0; i < validCellsY; i++)
	{
		for (int j = 0; j < validCellsX; j++)
		{
			visited[i][j] = false;
		}
	}

	//обозначить начало работы алгоритма в какой-нибудь точке
	//пусть это будет угол
	//задаём currentCell, имеющий координаты в матрице доступных клеток, а также 
	//currentX и currentY, координаты в исходной матрице
	temp = rand() % 4;
	if (temp == 0)
	{
		visited[0][0] = true;
		currentCell.y = 0;
		currentCell.x = 0;
		//координаты в лабиринте
		currentY = 1;
		currentX = 1;
	}
	else if (temp == 1)
	{
		visited[0][validCellsX - 1] = true;
		currentCell.y = 0;
		currentCell.x = validCellsX - 1;
		currentY = 1;
		currentX = width - 2;
	}
	else if (temp == 2)
	{
		visited[validCellsY - 1][0] = true;
		currentCell.y = validCellsY - 1;
		currentCell.x = 0;
		currentY = height - 2;
		currentX = 1;
	}
	else if (temp == 3)
	{
		visited[validCellsY - 1][validCellsX - 1] = true;
		currentCell.y = validCellsY - 1;
		currentCell.x = validCellsX - 1;
		currentY = height - 2;
		currentX = width - 2;
	}

	//первая точка - посещена!
	visitedCells = 1;

	//затем в цикле что делаем? заносим в список все соседние точки по четырём направлениям,
	//выбираем случайную из них и переходим в неё
	while (visitedCells < validCellsX * validCellsY)
	{
		//определить соседей
		nearby.size = 0;
		//проверить клетки по четырём сторонам от текущей клетки
		//сверху - проверяем, не выйдем ли за границы матрицы
		if (currentCell.y - 1 > -1)
		{
			if (!visited[currentCell.y - 1][currentCell.x]) //если в данной клетке посещения ещё не было - там стоит false
			{
				//клетка подходит
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x, currentCell.y - 1 };
			}
		}
		//аналогично для остальных четырёх сторон
		//справа
		if (currentCell.x + 1 < validCellsX)
		{
			if (!visited[currentCell.y][currentCell.x + 1])
			{
				//клетка подходит
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x + 1, currentCell.y };
			}
		}
		//снизу
		if (currentCell.y + 1 < validCellsY)
		{
			if (!visited[currentCell.y + 1][currentCell.x])
			{
				//клетка подходит
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x, currentCell.y + 1 };
			}
		}
		//слева
		if (currentCell.x - 1 > -1)
		{
			if (!visited[currentCell.y][currentCell.x - 1])
			{
				//клетка подходит
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x - 1, currentCell.y };
			}
		}

		if (nearby.size > 0)
		{
			//выбрать случайного соседа
			temp = rand() % nearby.size;
			//теперь текущая - одна из клеток массива соседей
			prevCell = currentCell;
			currentCell = nearby.cells[temp];
			//обозначить эту клетку как посещённую
			visited[currentCell.y][currentCell.x] = true;
			//сломать стену между этими клетками
			//сначала определить, куда двинулись
			//для этого сравниваем координаты текущей и предыдущей клетки
			//одновременно может измениться только одна координата
			if (currentCell.x != prevCell.x)
			{
				//произошло движение по горизонтали
				if (currentCell.x > prevCell.x)
				{
					//двинулись вправо
					maze[currentY][currentX + 1] = ' ';
					currentX += 2;
				}
				else
				{
					//двинулись влево
					maze[currentY][currentX - 1] = ' ';
					currentX -= 2;
				}
			}
			else
			{
				//произошло движение по вертикали
				if (currentCell.y > prevCell.y)
				{
					//двинулись вниз
					maze[currentY + 1][currentX] = ' ';
					currentY += 2;
				}
				else
				{
					//двинулись вверх
					maze[currentY - 1][currentX] = ' ';
					currentY -= 2;
				}
			}
			//посетили ещё одну клетку
			visitedCells++;
		}
		else
		{
			//если по соседству нет непосещённых клеток - ищем ту, где они есть
			flag = false;
			for (int i = 0; i < validCellsY && !flag; i++)
			{
				for (int j = 0; j < validCellsX && !flag; j++)
				{
					flag = false;
					if (visited[i][j] == true)
					{
						if (i + 1 < validCellsY)
							if (visited[i + 1][j] == false)
								flag = true;
						if (j + 1 < validCellsX)
							if (visited[i][j + 1] == false)
								flag = true;
						if (i - 1 > -1)
							if (visited[i - 1][j] == false)
								flag = true;
						if (j - 1 > -1)
							if (visited[i][j - 1] == false)
								flag = true;
						if (flag == true)
						{
							currentCell = { j, i };
							//переопределить координаты в лабиринте
							currentX = 2 * j + 1;
							currentY = 2 * i + 1;
						}
					}
				}
			}
		}

	}

}

void Maze::draw(RenderWindow& window, String tileset)
{
	//осуществляет отрисовку лабиринта в окно
	

	window.clear();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//код обработки строки карты: куча ёбаных ифов
			if (maze[i][j] == ' ')
				//проход
				mapSpr.setTextureRect(IntRect(0, 0, 16, 16));
			if (maze[i][j] == '*')
				//стена
				mapSpr.setTextureRect(IntRect(16, 0, 16, 16));
			mapSpr.setPosition(j * 16, i * 16);
			window.draw(mapSpr);
		}
	}
}