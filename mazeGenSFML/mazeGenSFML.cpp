// mazeGenSFML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "mazeGen.h"
#include <Windows.h>
#include <locale.h>
#include "Bot.h"
#include <TGUI/TGUI.hpp>

using namespace sf;
using namespace std;
using namespace tgui;

bool flag = false;
bool gotBack = false;

void getData()
{
	//получает данные из полей ввода на стартовом окне
	flag = true;
}

void goBack()
{
	//вернуться из главного окна программы на стартовое
	gotBack = true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	
	
	
	int Width, Height, Seed;
	int minMazeW = 11, minMazeH = 11;
	int Temp, tilesetCount = 4;

	//вывести стартовое окошко?
	//RenderWindow startWindow(sf::VideoMode(640, 768), "Start");

start:
	RenderWindow startWindow(sf::VideoMode(640, 768), "Start");
	Gui gui(startWindow);

	auto back = tgui::Picture::create({ "Images/blacky.jpg", { 0, 0, 640, 768 } });
	gui.add(back);

	auto toplabel = Label::create("Start");
	toplabel->setPosition(280, 50);
	toplabel->setHorizontalAlignment(Label::HorizontalAlignment::Right);
	toplabel->setTextColor(sf::Color(200, 200, 200));
	toplabel->setTextSize(36);
	gui.add(toplabel);

	auto midlabel = Label::create("Input labyrinth size");
	midlabel->setPosition(180, 150);
	midlabel->setHorizontalAlignment(Label::HorizontalAlignment::Center);
	midlabel->setTextColor(sf::Color(200, 200, 200));
	midlabel->setTextSize(30);
	gui.add(midlabel);

	auto leftlabel = Label::create("Width");
	leftlabel->setPosition(50, 250);
	leftlabel->setHorizontalAlignment(Label::HorizontalAlignment::Center);
	leftlabel->setTextColor(sf::Color(200, 200, 200));
	leftlabel->setTextSize(30);
	gui.add(leftlabel);

	auto rightlabel = Label::create("Height");
	rightlabel->setPosition(480, 250);
	rightlabel->setHorizontalAlignment(Label::HorizontalAlignment::Center);
	rightlabel->setTextColor(sf::Color(200, 200, 200));
	rightlabel->setTextSize(30);
	gui.add(rightlabel);

	tgui::EditBox::Ptr editBoxWidth = tgui::EditBox::create();
	editBoxWidth->setSize(150, 50);
	editBoxWidth->setPosition(30, 300);
	editBoxWidth->setDefaultText("11");
	editBoxWidth->setInputValidator(EditBox::Validator::Int);
	gui.add(editBoxWidth, "widthBox");

	tgui::EditBox::Ptr editBoxHeight = tgui::EditBox::create();
	editBoxHeight->setSize(150, 50);
	editBoxHeight->setPosition(455, 300);
	editBoxHeight->setDefaultText("11");
	editBoxHeight->setInputValidator(EditBox::Validator::Int);
	gui.add(editBoxHeight, "heightBox");

	auto midmidlabel = Label::create("Input generation seed");
	midmidlabel->setPosition(165, 400);
	midmidlabel->setHorizontalAlignment(Label::HorizontalAlignment::Center);
	midmidlabel->setTextColor(sf::Color(200, 200, 200));
	midmidlabel->setTextSize(30);
	gui.add(midmidlabel);

	tgui::EditBox::Ptr editBoxSeed = tgui::EditBox::create();
	editBoxSeed->setSize(150, 50);
	editBoxSeed->setPosition(250, 470);
	editBoxSeed->setDefaultText("42");
	editBoxSeed->setInputValidator(EditBox::Validator::Int);
	gui.add(editBoxSeed, "seedBox");

	auto btnCreate = tgui::Button::create();
	btnCreate->setPosition(220, 600);
	btnCreate->setSize(200, 50);
	btnCreate->setText("Create");
	gui.add(btnCreate);

	btnCreate->connect("pressed", getData);
	String HeightStr, WidthStr, SeedStr; //строки для записи значений из виджетов
	Width = 0;
	Height = 0;
	Seed = 42;
	//обработка стартового окна
	while (startWindow.isOpen())
	{
		sf::Event event;
		while (startWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				startWindow.close();

			}

			if (flag)
			{
				//нажата кнопка, можно ехать дальше
				WidthStr = editBoxWidth->getText();
				HeightStr = editBoxHeight->getText();
				SeedStr = editBoxSeed->getText();
				//вытащить инты из строки
				std::string wstr = WidthStr.toAnsiString();
				std::string hstr = HeightStr.toAnsiString();
				std::string sstr = SeedStr.toAnsiString();
				std::istringstream issw(wstr);
				std::istringstream issh(hstr);
				std::istringstream isss(sstr);
				issw >> Width;
				issh >> Height;
				isss >> Seed;
				startWindow.close();
				flag = false;
			}


			gui.handleEvent(event);
		}
		gui.draw();
		startWindow.display();
	}

	//

	if (Width < 11)
		Width = 11;
	if (Height < 11)
		Height = 11;
	if (Width % 2 == 0)
	{
		cout << "Для работы алгоритма нужна нечётная размерность! Прибавляем единицу.\n";
		Width++;
	}
	if (Height % 2 == 0)
	{
		cout << "Для работы алгоритма нужна нечётная размерность! Прибавляем единицу.\n";
		Height++;
	}
	//загенерить тайлсет
	srand(time(0));
	Temp = rand() % tilesetCount;
	String tileset;
	switch (Temp)
	{
	case 0:
		tileset = "Images/0.png";
		break;
	case 1:
		tileset = "Images/1.png";
		break;
	case 2:
		tileset = "Images/2.png";
		break;
	case 3:
		tileset = "Images/3.png";
		break;
	default:
		break;
	}
	//создать лабиринт
	Maze Mz(Width, Height, Seed, tileset);
	//произвести генерацию
	Mz.generateMaze();
	
	//создать ботов
	//инициализировать список доступных скинов для ботов и клеток для спавна, имена
	Bot::prepareValidCellsList(Mz);
	Bot::prepareSkins();
	Bot::prepareNames();
	//число ботов должно вычисляться в зависимости от размерности лабиринта
	//размерность лабиринта - w*h
	//положим, что ходить можно лишь по половине из этих клеток - wh/2
	//на каждого бота, допустим, нужно 50 клеток
	int botsCount = Width * Height / 2 / 50;
	//таким образом, на минимальной размерности в 121 блок будет 1 бот.

	vector<Bot> bots;
	bots.resize(botsCount);
	
	//создать окно
	//главное окно программы мазафака
	View view(FloatRect(200, 200, 300, 200));
	int mainWidth = 1024, mainHeight = 768;
	RenderWindow mainWindow(sf::VideoMode(mainWidth, mainHeight), "GETTING STARTED");
	Gui maingui(mainWindow);

	auto chatbox = ChatBox::create();
	chatbox->setSize(300, 100);
	chatbox->setTextSize(20);
	chatbox->setPosition(25, 25);
	maingui.add(chatbox);

	auto backbtn = Button::create();
	backbtn->setPosition(350, 25);
	backbtn->setSize(50, 50);
	backbtn->setText("Back");
	maingui.add(backbtn);

	backbtn->connect("pressed", goBack);
	view.reset(sf::FloatRect(0, 0, 640, 480));


	for (int i = 0; i < botsCount; i++)
	{
		bots[i].respawn(Mz);
		bots[i].navigate(Mz);
	}
	
	for (int i = 0; i < botsCount; i++)
	{
		cout << "Bot" << i << ": X = " << bots[i].X << ", Y = " << bots[i].Y << ", tileset: " << bots[i].tileset.toAnsiString() << "\n";
	}

	Clock clock;
	
	while (mainWindow.isOpen())
	{
		sf::Event event;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500; //было 800, не понравится - вернём
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();
			if (gotBack)
			{
				//возвращение...
				mainWindow.close();
				gotBack = false;
				goto start;
			}
			maingui.handleEvent(event);
		}


		sf::Vector2i localPosition = Mouse::getPosition(); //пихаем сюда координаты курсора


		if (localPosition.x < 3 && view.getCenter().x >= 150)
		{
			view.move(-0.2 * time, 0);
		}
		if (localPosition.x > mainWindow.getSize().x - 3 && view.getCenter().x <= 876)
		{
			view.move(0.2 * time, 0);
		}
		if (localPosition.y < 3 && view.getCenter().y >= 150)
		{
			view.move(0, -0.2 * time);
		}
		if (localPosition.y > mainWindow.getSize().y - 3 && view.getCenter().y <= 618)
		{
			view.move(0, 0.2 * time);
		}

		mainWindow.setView(view);
		mainWindow.clear();

		for (int i = 0; i < botsCount; i++)
			bots[i].update(time, Mz);
		//проверить, не вступили ли боты в схватку
		int sum; //сумма очков силы бойцов
		int randN; //случайное число
		//очистить виджет список ботов 
		chatbox->removeAllLines();
		for (int i = 0; i < botsCount; i++)
		{

			for (int k = i + 1; k < botsCount; k++)
			{

				if (bots[i].x == bots[k].x && bots[i].y == bots[k].y && bots[i].alive && bots[k].alive)
				{
					//И ВНОВЬ ПРОДОЛЖАЕТСЯ БОЙ
					//И СЕРДЦУ ТРЕВОЖНО В ГРУДИ
					//И ЛЕНИН
					//ТАКОЙ МОЛОДОЙ
					//И ЮНЫЙ ОКТЯБРЬ ВПЕРЕДИ

					//короче, боты, находящиеся на одной клетке, пиздяца
					sum = bots[i].strength + bots[k].strength;
					//бросить кубик
					randN = rand() % sum;
					if (randN <= bots[i].strength)
					{
						//первый бот победил, прямо как пластмассовый мир
						bots[i].eat();
						bots[k].die();
					}
					else
					{
						//победил второй бот
						bots[k].eat();
						bots[i].die();
					}
				}
			}
		}
		//обновить список ботов в виджете
		for (int i = 0; i < botsCount; i++)
		{
			std::stringstream ss;
			string str;
			string nameStr = bots[i].name.toAnsiString();
			ss << nameStr << "_" << bots[i].frags << "/" << bots[i].deaths << ",_";
			bots[i].alive ? ss << "alive" : ss << "dead";
			ss >> str;
			sf::String Str(str);
			chatbox->addLine(Str, sf::Color::Red);
		}
		Mz.draw(mainWindow, tileset);
		for (int i = 0; i < botsCount; i++)
			if (bots[i].alive)
				mainWindow.draw(bots[i].sprite);

		maingui.draw();
		mainWindow.display();
	}

	return 0;
}

