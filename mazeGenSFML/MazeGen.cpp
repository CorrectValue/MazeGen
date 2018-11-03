#include "stdafx.h"
#include "mazeGen.h"

void Maze::generateMaze()
{
	//����� ������ �������� � ����� ������� ���������


	//������ ��� ���������
	srand(seed);

	//������ ����� ��������� ������ ���������
	maze = new char*[height];

	for (int i = 0; i < height; i++)
	{
		maze[i] = new char[width];
	}

	//�������� ������� ���: ��� ��������� ������ ������� �������
	//������ �����!
	for (int i = 0; i < height; i++)
	{
		//�������� �� �������������� ������
		if (i % 2 == 0)
		{
			//������ ����� ����
			for (int j = 0; j < width; j++)
			{
				maze[i][j] = '*';
			}
		}
		else
			//������ ����� ��������
			for (int j = 0; j < width; j++)
			{
				maze[i][j] = ' ';
			}
	}

	//���������� ������ ������������ �����
	for (int i = 0; i < width; i++)
	{
		if (i % 2 == 0)
		{
			//������ ����� - �����
			for (int j = 0; j < height; j++)
			{
				maze[j][i] = '*';
			}
		}
		else
			//�������� ����� - ������... ������ ������� ��������������� �����!
			for (int j = 0; j < height; j++)
			{
				if (maze[j][i] != '*')
					maze[j][i] = ' ';
			}
	}

	//������ ������� ���������� �����
	//���������, ������� �������� ��������� ����� ������
	int validCellsX, validCellsY;
	validCellsX = width / 2; //����������� � ������� �������, ��?
	validCellsY = height / 2;

	//���������������� ������� ���������
	visited = new bool*[validCellsY];
	for (int i = 0; i < validCellsY; i++)
		visited[i] = new bool[validCellsX];
	//���������� ��� ������� �� ��������
	for (int i = 0; i < validCellsY; i++)
	{
		for (int j = 0; j < validCellsX; j++)
		{
			visited[i][j] = false;
		}
	}

	//���������� ������ ������ ��������� � �����-������ �����
	//����� ��� ����� ����
	//����� currentCell, ������� ���������� � ������� ��������� ������, � ����� 
	//currentX � currentY, ���������� � �������� �������
	temp = rand() % 4;
	if (temp == 0)
	{
		visited[0][0] = true;
		currentCell.y = 0;
		currentCell.x = 0;
		//���������� � ���������
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

	//������ ����� - ��������!
	visitedCells = 1;

	//����� � ����� ��� ������? ������� � ������ ��� �������� ����� �� ������ ������������,
	//�������� ��������� �� ��� � ��������� � ��
	while (visitedCells < validCellsX * validCellsY)
	{
		//���������� �������
		nearby.size = 0;
		//��������� ������ �� ������ �������� �� ������� ������
		//������ - ���������, �� ������ �� �� ������� �������
		if (currentCell.y - 1 > -1)
		{
			if (!visited[currentCell.y - 1][currentCell.x]) //���� � ������ ������ ��������� ��� �� ���� - ��� ����� false
			{
				//������ ��������
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x, currentCell.y - 1 };
			}
		}
		//���������� ��� ��������� ������ ������
		//������
		if (currentCell.x + 1 < validCellsX)
		{
			if (!visited[currentCell.y][currentCell.x + 1])
			{
				//������ ��������
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x + 1, currentCell.y };
			}
		}
		//�����
		if (currentCell.y + 1 < validCellsY)
		{
			if (!visited[currentCell.y + 1][currentCell.x])
			{
				//������ ��������
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x, currentCell.y + 1 };
			}
		}
		//�����
		if (currentCell.x - 1 > -1)
		{
			if (!visited[currentCell.y][currentCell.x - 1])
			{
				//������ ��������
				nearby.size += 1;
				nearby.cells[nearby.size - 1] = { currentCell.x - 1, currentCell.y };
			}
		}

		if (nearby.size > 0)
		{
			//������� ���������� ������
			temp = rand() % nearby.size;
			//������ ������� - ���� �� ������ ������� �������
			prevCell = currentCell;
			currentCell = nearby.cells[temp];
			//���������� ��� ������ ��� ����������
			visited[currentCell.y][currentCell.x] = true;
			//������� ����� ����� ����� ��������
			//������� ����������, ���� ���������
			//��� ����� ���������� ���������� ������� � ���������� ������
			//������������ ����� ���������� ������ ���� ����������
			if (currentCell.x != prevCell.x)
			{
				//��������� �������� �� �����������
				if (currentCell.x > prevCell.x)
				{
					//��������� ������
					maze[currentY][currentX + 1] = ' ';
					currentX += 2;
				}
				else
				{
					//��������� �����
					maze[currentY][currentX - 1] = ' ';
					currentX -= 2;
				}
			}
			else
			{
				//��������� �������� �� ���������
				if (currentCell.y > prevCell.y)
				{
					//��������� ����
					maze[currentY + 1][currentX] = ' ';
					currentY += 2;
				}
				else
				{
					//��������� �����
					maze[currentY - 1][currentX] = ' ';
					currentY -= 2;
				}
			}
			//�������� ��� ���� ������
			visitedCells++;
		}
		else
		{
			//���� �� ��������� ��� ������������ ������ - ���� ��, ��� ��� ����
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
							//�������������� ���������� � ���������
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
	//������������ ��������� ��������� � ����
	

	window.clear();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//��� ��������� ������ �����: ���� ������ ����
			if (maze[i][j] == ' ')
				//������
				mapSpr.setTextureRect(IntRect(0, 0, 16, 16));
			if (maze[i][j] == '*')
				//�����
				mapSpr.setTextureRect(IntRect(16, 0, 16, 16));
			mapSpr.setPosition(j * 16, i * 16);
			window.draw(mapSpr);
		}
	}
}