#include <SFML/Graphics.hpp>	
#include <iostream>
#include <vector>
#include <fstream>
#include <Country.h>
#include <Picture.h>
#include <Camera.h>
#include <Menu.h>
using namespace std;
using namespace sf;

Vector2i winSize(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
Vector2f mapSize(2880, 1800);

int viewSpeed = 15;
float wheel = 1;
int Mouse_x, Mouse_y;

int curCountry = -1;
int couSize = 0;

int main()
{
	RenderWindow window(VideoMode(winSize.x, winSize.y), "world", Style::Fullscreen); 
	Country* countries[100];

	string file_s; unsigned file_x, file_y; float file_p;
	
	//чтение из БД с названиями и координатами спрайтов для стран и запись данных в массив
	ifstream file("data/database.txt");
	while(file >> file_s >> file_x >> file_y >> file_p)
	{
		countries[couSize] = new Country(file_s, file_x, file_y, 0, file_p);
		couSize++;
	}
	file.close();


	Font font;
	font.loadFromFile("LemonMilkRegular-X3XE2.otf");
	Text load("", font, 50);							//после, используется для экрана загрузки
	load.setPosition(winSize.x / 2, winSize.y / 2);

	//анализ и запись данных из датасета в массив
	file.open("data/covid_dataset.txt"); 
	string file2_s,skip; unsigned day, month, year, cases, deaths;
	unsigned long long population;
	getline(file, file2_s); 
	
	int count=0;
	while (count < 61000)
	{
		if (count % 100 == 0)
		{
			load.setString("Loading: " + to_string(count * 100 / 61000) + '%');				
			load.setOrigin(load.getLocalBounds().width / 2.0f, load.getLocalBounds().height / 2.0f);
			window.clear();
			window.draw(load);
			window.display();				//показывает уровень выполения анализа датасета
		}
		file >> file2_s >> day >> month >> year >> cases >> deaths >> file2_s >> skip >> skip >> population;
		for (int i = 0; i < couSize; i++)
		{
			if (countries[i]->name == file2_s)
			{
				countries[i]->population = population;
				countries[i]->push_record(day, month, year, cases, deaths);
				countries[i]->cases_total += cases;
				countries[i]->deaths_total += deaths;
			}
		}
		getline(file, file2_s); count++;
	} 
	file.close();

	window.setVerticalSyncEnabled(true);	//устанавливает ограничение кадров в секунду
	
	Picture main_map("maps", 0, 0);
	Menu menu;
	Clock clock;
	Time time;
	Camera camera(mapSize, winSize);
	Event event;

	bool graphShow = 0;
	IntRect graphBtn(winSize.x-181, winSize.y/2+270, 181, 37);

	RectangleShape blackBG;
	blackBG.setSize(Vector2f(2880, 1800));
	blackBG.setFillColor(Color(0, 0, 0, 100));

	while (window.isOpen())		//главный цикл программы
	{
		while (window.pollEvent(event)) //цикл событий
		{
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left) //проверка нажатий на объекты
				{
					Mouse_x = camera.Cx + (Mouse::getPosition().x - winSize.x/2) * camera.wheel * mapSize.x / winSize.x;
					Mouse_y = camera.Cy + (Mouse::getPosition().y - winSize.y/2) * camera.wheel * mapSize.y / winSize.y;
					if(graphBtn.contains(Mouse::getPosition()))
					{
						graphShow = !graphShow;
					}
					for (int i = 0; i < couSize; ++i)
					{
						if (countries[i]->isSpritePressed(Mouse_x, Mouse_y))
						{
							if (curCountry >= 0 && curCountry != i)
							{
								clock.restart();
							}
							curCountry = i;
						}
					}
				}
			}

			//Зум
			if (event.type == Event::MouseWheelMoved)
			{
				camera.scale(event.mouseWheel.delta);
			}

			if (event.type == Event::KeyPressed)
			{
				switch(event.key.code)
				{
				case Keyboard::Escape:	//выход из приложения при нажатии Escape
					window.close();
					return 0;
					break;
				}
			}

			if (event.type == Event::Closed)
			{
				window.close();
				return 0;
			}
		}

		camera.move(Mouse::getPosition());
		window.setView(camera.view);		//перемещение мышкой по карте
		
		time = clock.getElapsedTime();
		if (time.asSeconds() >= 1.57f) clock.restart();	//таймер используется в циклический анимациях (подсветка страны)

		window.clear();					// "рисование" нужных объектов
		window.draw(main_map.sprite);
		if (curCountry >= 0)
		{
			countries[curCountry]->animation(time.asSeconds());
			window.draw(countries[curCountry]->sprite);
			if (graphShow) 
			{
				window.draw(blackBG);
				window.draw(menu.drawGraph(countries[curCountry], camera.Cx, camera.Cy, camera.wheel, mapSize)); 
			}
			window.draw(menu.drawMenu(countries[curCountry], camera.Cx, camera.Cy, camera.wheel, mapSize, graphShow));
		}
		window.draw(menu.drawAppName(camera.Cx, camera.Cy, camera.wheel, mapSize));
		window.display();
	}
}




//some algorithms used during the work process (некоторые алгосы для тестинга)
/*ifstream file("data/database.txt");
	string s; int i = 0;
	vector<string> kek;
	while (!file.eof())
	{
		s.erase();
		getline(file, s);
		i = s.length() - 1;
		while (isdigit((unsigned char)s[i]) || s[i] == '.') { i--; }
		while (!isdigit((unsigned char)s[i]) || s[i] == '.') { i--; }
		s.insert(i + 1, "000");
		kek.push_back(s);
	}file.close(); sort(kek.begin(),kek.end());
	ofstream fou("data/database.txt");
	for (i = 0; i < kek.size(); i++) { fou <<"1 "+kek[i]+'\n'; }
	fou.close();

	/*
	ifstream file("data.txt");
	string l, s,p; int i = 0; int ou = 0;
	getline(file, l); bool b = 0,pt=0;
	vector<string> kek; kek.push_back("og");
	while (!file.eof())
	{
		s.erase(); p.erase(); b = 0;
		getline(file, l);
		for (i = 22; i < l.length(); i++) {
			if (isalpha((unsigned char)l[i])) {
				b = 1;
			} if (b)break;
		}
		while (isalpha((unsigned char)l[i])||l[i]=='_') {
			s.push_back(l[i]); i++;
		}
		while (!isdigit((unsigned char)l[i])) i++;
		while (!isalpha((unsigned char)l[i])) {  i++; }
		while (isalpha((unsigned char)l[i]) || l[i] == '_') {
			p.push_back(l[i]); i++;
		}
		if((s + '\t' + p )!=kek.back())kek.push_back(s+'\t'+p);

	}cout << "knltn";
	ofstream fou("lol.txt");
	for (i = 0; i < kek.size(); i++) { fou << kek[i] << '\n'; }
	file.close(); */